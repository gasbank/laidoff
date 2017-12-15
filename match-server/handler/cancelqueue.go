package handler

import (
	"net"
	"log"
	"github.com/gasbank/laidoff/db-server/user"
	"github.com/gasbank/laidoff/match-server/convert"
	"github.com/gasbank/laidoff/match-server/battle"
	"github.com/gasbank/laidoff/db-server/dbservice"
)

func HandleCancelQueue(matchQueue chan<- user.Agent, buf []byte, conn net.Conn, ongoingBattleMap map[user.Id]battle.Ok, dbService dbservice.Db) {
	log.Printf("CANCELQUEUE received")
	recvPacket, err := convert.ParseCancelQueue(buf)
	if err != nil {
		log.Printf("HandleCancelQueue fail: %v", err.Error())
		return
	}
	//userDb, err := user.LoadUserDb(convert.IdCuintToByteArray(recvPacket.Id))
	var userDb user.Db
	userId := convert.IdCuintToByteArray(recvPacket.Id)
	err = dbService.Get(&userId, &userDb)
	if err != nil {
		log.Printf("user db load failed: %v", err.Error())
	} else {
		// Check ongoing battle
		_, battleExists := ongoingBattleMap[userDb.Id]
		if battleExists {
			log.Printf("Nickname '%v' has the ongoing battle session. CANCELQUEUE?!", userDb.Nickname)
		}
		// Queue connection
		matchQueue <- user.Agent{conn, userDb, true }
	}
}
