package handler

import (
	"bytes"
	"log"
	"unsafe"
	"time"
	"fmt"
	"net"
	"../service"
	"../convert"
)
// #include "../../src/puckgamepacket.h"
import "C"

func HandlePushToken(buf []byte, conn net.Conn, serviceList *service.ServiceList) {
	log.Printf("PUSHTOKEN received")
	// Parse
	recvPacket, err := convert.ParsePushToken(buf)
	if err != nil {
		log.Printf("HandlePushToken fail: %v", err.Error())
	}
	idBytes := convert.IdCuintToByteArray(recvPacket.Id)
	pushTokenBytes := C.GoBytes(unsafe.Pointer(&recvPacket.Push_token), C.LW_PUSH_TOKEN_LENGTH)
	pushTokenLength := bytes.IndexByte(pushTokenBytes, 0)
	pushToken := string(pushTokenBytes[:pushTokenLength])
	log.Printf("Push token domain %v, token: %v, id: %v", recvPacket.Domain, pushToken, idBytes)
	pushResult := serviceList.Arith.RegisterPushToken(300*time.Millisecond, idBytes, int(recvPacket.Domain), pushToken)
	log.Printf("Push result: %v", pushResult)
	if pushResult == 1 {
		sysMsg := []byte(fmt.Sprintf("토큰 등록 완료! %v", pushToken))
		queueOkBuf := convert.Packet2Buf(convert.NewSysMsg(sysMsg))
		conn.Write(queueOkBuf)
	}
}
