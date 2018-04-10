package main

import "C"
import (
	"fmt"
	"net"
	"os"
	"encoding/binary"
	"log"
	"github.com/gasbank/laidoff/match-server/convert"
	"unsafe"
	"path/filepath"
)

/*
#include <stdio.h>
#include <stdlib.h>
typedef struct _LWREMTEXTEXPART {
    unsigned int name_hash;
    unsigned int total_size;
    unsigned int offset;
    unsigned char data[1024];
} LWREMTEXTEXPART;

unsigned long hash2(const unsigned char *str) {
    unsigned long hash = 5381;
    int c;

    while ((c = *str++))
        hash = ((hash << 5) + hash) + c; // hash * 33 + c

	return hash;
}
unsigned long hash(const char *str) {
	return hash2((const unsigned char*)str);
}
*/
import "C"

/* A Simple function to verify error */
func checkError(err error) {
	if err != nil {
		fmt.Println("Error: ", err)
		os.Exit(0)
	}
}

func hash(s string) uint32 {
	cStr := C.CString(s)
	h := C.hash(cStr)
	C.free(unsafe.Pointer(cStr))
	return uint32(h)
}

func main() {
	if len(os.Args) < 2 {
		log.Printf("Should provide resource glob as first argument. (i.e. c:/laidoff/assets/ktx/*.ktx)")
		return
	}
	/* Lets prepare a address at any address at port 10001*/
	serverAddr, err := net.ResolveUDPAddr("udp", ":19876")
	checkError(err)

	/* Now listen at selected port */
	serverConn, err := net.ListenUDP("udp", serverAddr)
	checkError(err)
	defer serverConn.Close()

	buf := make([]byte, 1024)

	files, _ := filepath.Glob(os.Args[1])
	fileMap := make(map[uint32]string)
	fileCacheMap := make(map[uint32][]byte)
	log.Printf("Total files: %v", len(files))
	for _, f := range files {
		filename := filepath.Base(f)
		name := filename[:len(filename)-len(filepath.Ext(filename))]
		h := hash(name)
		fileMap[h] = f
		log.Printf("filename: %v, hash: %v", name, h)
	}

	for {
		n, addr, err := serverConn.ReadFromUDP(buf)

		if err != nil {
			fmt.Println("Error: ", err)
		}

		if n == 8 {
			nameHash := binary.LittleEndian.Uint32(buf[0:4])
			offset := binary.LittleEndian.Uint32(buf[4:8])
			//log.Printf("Received %v bytes from %v: nameHash %v, offset %v", n, addr, nameHash, offset)

			// check cache
			filename := fileMap[nameHash]
			if _, ok := fileCacheMap[nameHash]; ok {
			} else {
				log.Printf("%v: Not cached file.", filename)
				filename := fileMap[nameHash]
				file, err := os.Open(filename) // For read access.
				if err != nil {
					log.Fatalf(err.Error())
				}
				fileStat, err := file.Stat()
				if err != nil {
					log.Fatalf(err.Error())
				}
				totalSize := fileStat.Size()
				fileCacheMap[nameHash] = make([]byte, totalSize)
				_, err = file.Read(fileCacheMap[nameHash])
				if err != nil {
					log.Fatalf(err.Error())
				}
				log.Printf("%v: Saved. Total size %v bytes.", filename, len(fileCacheMap[nameHash]))
			}
			fileData := fileCacheMap[nameHash]
			totalSize := len(fileData)
			payloadSize := uint32(0)
			remained := uint32(totalSize) - offset
			if remained < 1024 {
				payloadSize = remained
			} else {
				payloadSize = 1024
			}

			texPart := C.LWREMTEXTEXPART{}
			texPart.name_hash = C.uint(nameHash)
			texPart.total_size = C.uint(totalSize)
			texPart.offset = C.uint(offset)
			for i := uint32(0); i < payloadSize; i++ {
				texPart.data[i] = C.uchar(fileData[offset + i])
			}

			replyBuf := convert.Packet2Buf(texPart)
			//log.Printf("Replying %v bytes to %v: file %v total %v offset %v, payloadSize %v", len(replyBuf), addr, filename, totalSize, offset, payloadSize)
			serverConn.WriteToUDP(replyBuf, addr)
		} else {
			log.Printf("Unknown size: %v", n)
		}
	}
}
