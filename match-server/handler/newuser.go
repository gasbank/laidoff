package handler

import (
	"net"
	"log"
	"github.com/gasbank/laidoff/match-server/user"
	"github.com/gasbank/laidoff/match-server/nickdb"
	"github.com/gasbank/laidoff/match-server/convert"
)

func HandleNewUser(nickDb *Nickdb.NickDb, conn net.Conn) {
	log.Printf("NEWUSER received")
	uuid, uuidStr, err := user.NewUuid()
	if err != nil {
		log.Fatalf("new uuid failed: %v", err.Error())
	}
	log.Printf("  - New user guid: %v", uuidStr)
	newNick := Nickdb.PickRandomNick(nickDb)
	newUserDataBuf := convert.Packet2Buf(convert.NewLwpNewUserData(uuid, newNick))
	// Write to disk
	var id user.Id
	copy(id[:], uuid)
	_, _, err = user.CreateNewUser(id, newNick)
	if err != nil {
		log.Fatalf("CreateNewUser failed: %v", err.Error())
	}
	_, err = conn.Write(newUserDataBuf)
	if err != nil {
		log.Fatalf("NEWUSERDATA send failed: %v", err.Error())
	}
}
