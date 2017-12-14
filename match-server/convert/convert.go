package convert

import (
	"encoding/binary"
	"fmt"
	"github.com/gasbank/laidoff/db-server/user"
	"unsafe"
	"bytes"
	"net"
	"github.com/gasbank/laidoff/shared-server"
	"log"
)
// #include "../../src/puckgamepacket.h"
import "C"

/////////////////////////////////////////////////////////////////////////////////////
// SECTION: Receiving Packet Enums
/////////////////////////////////////////////////////////////////////////////////////
//noinspection ALL
const (
	LPGPLWPQUEUE2         = C.LPGP_LWPQUEUE2
	LPGPLWPCANCELQUEUE    = C.LPGP_LWPCANCELQUEUE
	LPGPLWPSUDDENDEATH    = C.LPGP_LWPSUDDENDEATH
	LPGPLWPNEWUSER        = C.LPGP_LWPNEWUSER
	LPGPLWPQUERYNICK      = C.LPGP_LWPQUERYNICK
	LPGPLWPPUSHTOKEN      = C.LPGP_LWPPUSHTOKEN
	LPGPLWPGETLEADERBOARD = C.LPGP_LWPGETLEADERBOARD
	LPGPLWPSETNICKNAME    = C.LPGP_LWPSETNICKNAME
	LPGPLWPBATTLERESULT   = C.LPGP_LWPBATTLERESULT
)

/////////////////////////////////////////////////////////////////////////////////////
// SECTION: Packet Wrappers
/////////////////////////////////////////////////////////////////////////////////////
type CreateBattleOk struct {
	S C.LWPCREATEBATTLEOK
}

/////////////////////////////////////////////////////////////////////////////////////
// SECTION:  Packets
/////////////////////////////////////////////////////////////////////////////////////
func NewLwpBattleValid() (*C.LWPBATTLEVALID, int) {
	return &C.LWPBATTLEVALID{}, int(C.LPGP_LWPBATTLEVALID)
}

func NewLwpCreateBattleOk() (*C.LWPCREATEBATTLEOK, int) {
	return &C.LWPCREATEBATTLEOK{}, int(C.LPGP_LWPCREATEBATTLEOK)
}

func NewLwpBattleResult() (*C.LWPBATTLERESULT, int) {
	return &C.LWPBATTLERESULT{}, int(C.LPGP_LWPBATTLERESULT)
}

func NewLwpRetryQueueLater() *C.LWPRETRYQUEUELATER {
	return &C.LWPRETRYQUEUELATER{
		C.ushort(unsafe.Sizeof(C.LWPRETRYQUEUELATER{})),
		C.LPGP_LWPRETRYQUEUELATER,
	}
}

func NewLwpRetryQueue() *C.LWPRETRYQUEUE {
	return &C.LWPRETRYQUEUE{
		C.ushort(unsafe.Sizeof(C.LWPRETRYQUEUE{})),
		C.LPGP_LWPRETRYQUEUE,
	}
}

func NewLwpCancelQueueOk() *C.LWPCANCELQUEUEOK {
	return &C.LWPCANCELQUEUEOK{
		C.ushort(unsafe.Sizeof(C.LWPCANCELQUEUEOK{})),
		C.LPGP_LWPCANCELQUEUEOK,
	}
}
func NewLwpMaybeMatched() *C.LWPMAYBEMATCHED {
	return &C.LWPMAYBEMATCHED{
		C.ushort(unsafe.Sizeof(C.LWPMAYBEMATCHED{})),
		C.LPGP_LWPMAYBEMATCHED,
	}
}
func NewLwpNick(nick string) *C.LWPNICK {
	cNewNickBytes := NicknameToCArray(nick)
	return &C.LWPNICK{
		C.ushort(unsafe.Sizeof(C.LWPNICK{})),
		C.LPGP_LWPNICK,
		cNewNickBytes,
	}
}
func NewLwpQueueOk() *C.LWPQUEUEOK {
	return &C.LWPQUEUEOK{
		C.ushort(unsafe.Sizeof(C.LWPQUEUEOK{})),
		C.LPGP_LWPQUEUEOK,
	}
}
func NewLwpSetNicknameResult(request *C.LWPSETNICKNAME) *C.LWPSETNICKNAMERESULT {
	return &C.LWPSETNICKNAMERESULT{
		C.ushort(unsafe.Sizeof(C.LWPSETNICKNAMERESULT{})),
		C.LPGP_LWPSETNICKNAMERESULT,
		request.Id,
		request.Nickname,
	}
}

func NewLwpMatched2(port int, ipv4 net.IP, battleId int, token uint, playerNo int, targetNickname string) *C.LWPMATCHED2 {
	return &C.LWPMATCHED2{
		C.ushort(unsafe.Sizeof(C.LWPMATCHED2{})),
		C.LPGP_LWPMATCHED2,
		C.ushort(port), // createBattleOk.Port
		C.ushort(0),    // padding
		[4]C.uchar{C.uchar(ipv4[0]), C.uchar(ipv4[1]), C.uchar(ipv4[2]), C.uchar(ipv4[3]),},
		C.int(battleId),
		C.uint(token),
		C.int(playerNo),
		NicknameToCArray(targetNickname),
	}
}

func NewLwpLeaderboard(leaderboardReply *shared_server.LeaderboardReply) *C.LWPLEADERBOARD {
	var nicknameList [C.LW_LEADERBOARD_ITEMS_IN_PAGE][C.LW_NICKNAME_MAX_LEN]C.char
	var scoreList [C.LW_LEADERBOARD_ITEMS_IN_PAGE]C.int
	for i, item := range leaderboardReply.Items {
		GoStringToCCharArray(&item.Nickname, &nicknameList[i])
		scoreList[i] = C.int(item.Score)
	}
	return &C.LWPLEADERBOARD{
		C.ushort(unsafe.Sizeof(C.LWPLEADERBOARD{})),
		C.LPGP_LWPLEADERBOARD,
		C.int(len(leaderboardReply.Items)),
		C.int(leaderboardReply.FirstItemRank),
		C.int(leaderboardReply.FirstItemTieCount),
		nicknameList,
		scoreList,
	}
}
func NewLwpNewUserData(uuid []byte, newNick string) *C.LWPNEWUSERDATA {
	cNewNickBytes := NicknameToCArray(newNick)
	return &C.LWPNEWUSERDATA{
		C.ushort(unsafe.Sizeof(C.LWPNEWUSERDATA{})),
		C.LPGP_LWPNEWUSERDATA,
		[4]C.uint{C.uint(binary.BigEndian.Uint32(uuid[0:4])), C.uint(binary.BigEndian.Uint32(uuid[4:8])), C.uint(binary.BigEndian.Uint32(uuid[8:12])), C.uint(binary.BigEndian.Uint32(uuid[12:16]))},
		cNewNickBytes,
	}
}
func NewLwpSysMsg(sysMsg []byte) *C.LWPSYSMSG {
	return &C.LWPSYSMSG{
		C.ushort(unsafe.Sizeof(C.LWPSYSMSG{})),
		C.LPGP_LWPSYSMSG,
		ByteArrayToCcharArray256(sysMsg),
	}
}

/////////////////////////////////////////////////////////////////////////////////////
// SECTION: Convert Utility Functions
/////////////////////////////////////////////////////////////////////////////////////
//func Int2ByteArray(v C.int) [4]byte {
//	var byteArray [4]byte
//	binary.LittleEndian.PutUint32(byteArray[0:], uint32(v))
//	return byteArray
//}

func GoStringToCCharArray(strIn *string, strOut *[C.LW_NICKNAME_MAX_LEN]C.char) {
	for i, b := range []byte(*strIn) {
		(*strOut)[i] = C.char(b)
	}
}

func CCharArrayToGoString(strIn *[C.LW_NICKNAME_MAX_LEN]C.char, strOut *string) {
	b := make([]byte, len(strIn))
	lastIndex := 0
	for i, ch := range strIn {
		b[i] = byte(ch)
		if b[i] == 0 {
			lastIndex = i
			break
		}
	}
	*strOut = string(b[:lastIndex])
}

func ByteArrayToCcharArray256(b []byte) [256]C.char {
	var ret [256]C.char
	bLen := len(b)
	if bLen > 255 {
		bLen = 255
	}
	for i := 0; i < bLen; i++ {
		ret[i] = C.char(b[i])
	}
	return ret
}

func UserIdToCuint(id user.Id) [4]C.uint {
	var b [4]C.uint
	b[0] = C.uint(binary.BigEndian.Uint32(id[0:4]))
	b[1] = C.uint(binary.BigEndian.Uint32(id[4:8]))
	b[2] = C.uint(binary.BigEndian.Uint32(id[8:12]))
	b[3] = C.uint(binary.BigEndian.Uint32(id[12:16]))
	return b
}

func IdCuintToByteArray(id [4]C.uint) user.Id {
	var b user.Id
	binary.BigEndian.PutUint32(b[0:], uint32(id[0]))
	binary.BigEndian.PutUint32(b[4:], uint32(id[1]))
	binary.BigEndian.PutUint32(b[8:], uint32(id[2]))
	binary.BigEndian.PutUint32(b[12:], uint32(id[3]))
	return b
}

func NicknameToCArray(nickname string) [C.LW_NICKNAME_MAX_LEN]C.char {
	var nicknameCchar [C.LW_NICKNAME_MAX_LEN]C.char
	for i, v := range []byte(nickname) {
		if i >= C.LW_NICKNAME_MAX_LEN {
			break
		}
		nicknameCchar[i] = C.char(v)
	}
	nicknameCchar[C.LW_NICKNAME_MAX_LEN-1] = 0
	return nicknameCchar
}

//func IdArrayToString(id [4]C.uint) string {
//	return fmt.Sprintf("%08x-%08x-%08x-%08x", id[0], id[1], id[2], id[3])
//}

func NewCreateBattle(id1, id2 user.Id, nickname1, nickname2 string) *C.LWPCREATEBATTLE {
	var c1Nickname [C.LW_NICKNAME_MAX_LEN]C.char
	var c2Nickname [C.LW_NICKNAME_MAX_LEN]C.char
	GoStringToCCharArray(&nickname1, &c1Nickname)
	GoStringToCCharArray(&nickname2, &c2Nickname)
	return &C.LWPCREATEBATTLE{
		C.ushort(unsafe.Sizeof(C.LWPCREATEBATTLE{})),
		C.LPGP_LWPCREATEBATTLE,
		UserIdToCuint(id1),
		UserIdToCuint(id2),
		c1Nickname,
		c2Nickname,
	}
}

func NewCheckBattleValid(battleId int) *C.LWPCHECKBATTLEVALID {
	return &C.LWPCHECKBATTLEVALID{
		C.ushort(unsafe.Sizeof(C.LWPCHECKBATTLEVALID{})),
		C.LPGP_LWPCHECKBATTLEVALID,
		C.int(battleId),
	}
}

func Packet2Buf(packet interface{}) []byte {
	buf := &bytes.Buffer{}
	binary.Write(buf, binary.LittleEndian, packet)
	return buf.Bytes()
}

func PushTokenBytes(recvPacket *C.LWPPUSHTOKEN) []byte {
	return C.GoBytes(unsafe.Pointer(&recvPacket.Push_token), C.LW_PUSH_TOKEN_LENGTH)
}

/////////////////////////////////////////////////////////////////////////////////////
// SECTION: Parse Packets
/////////////////////////////////////////////////////////////////////////////////////
func ParsePushToken(buf []byte) (*C.LWPPUSHTOKEN, error) {
	bufReader := bytes.NewReader(buf)
	recvPacket := &C.LWPPUSHTOKEN{}
	err := binary.Read(bufReader, binary.LittleEndian, recvPacket)
	if err != nil {
		log.Printf("binary.Read fail: %v", err.Error())
		return nil, err
	}
	return recvPacket, nil
}

func ParseQueryNick(buf []byte) (*C.LWPQUERYNICK, error) {
	bufReader := bytes.NewReader(buf)
	recvPacket := &C.LWPQUERYNICK{}
	err := binary.Read(bufReader, binary.LittleEndian, recvPacket)
	if err != nil {
		log.Printf("binary.Read fail: %v", err.Error())
		return nil, err
	}
	return recvPacket, nil
}

func ParseQueue2(buf []byte) (*C.LWPQUEUE2, error) {
	bufReader := bytes.NewReader(buf)
	recvPacket := &C.LWPQUEUE2{}
	err := binary.Read(bufReader, binary.LittleEndian, recvPacket)
	if err != nil {
		log.Printf("binary.Read fail: %v", err.Error())
		return nil, err
	}
	return recvPacket, nil
}

func ParseCancelQueue(buf []byte) (*C.LWPCANCELQUEUE, error) {
	bufReader := bytes.NewReader(buf)
	recvPacket := &C.LWPCANCELQUEUE{}
	err := binary.Read(bufReader, binary.LittleEndian, recvPacket)
	if err != nil {
		log.Printf("binary.Read fail: %v", err.Error())
		return nil, err
	}
	return recvPacket, nil
}

func ParseSetNickname(buf []byte) (*C.LWPSETNICKNAME, error) {
	bufReader := bytes.NewReader(buf)
	recvPacket := &C.LWPSETNICKNAME{}
	err := binary.Read(bufReader, binary.LittleEndian, recvPacket)
	if err != nil {
		log.Printf("binary.Read fail: %v", err.Error())
		return nil, err
	}
	return recvPacket, nil
}

func ParseGetLeaderboard(buf []byte) (*C.LWPGETLEADERBOARD, error) {
	bufReader := bytes.NewReader(buf)
	recvPacket := &C.LWPGETLEADERBOARD{}
	err := binary.Read(bufReader, binary.LittleEndian, recvPacket)
	if err != nil {
		log.Printf("binary.Read fail: %v", err.Error())
		return nil, err
	}
	return recvPacket, nil
}
