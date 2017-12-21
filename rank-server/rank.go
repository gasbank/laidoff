package main

import (
	"sort"
	"log"
	"errors"
	"net/rpc"
	"github.com/gasbank/laidoff/shared-server"
	"net"
	"fmt"
)

type UserId [16]byte

// RankData is a struct containing a single leaderboard.
type RankData struct {
	IdScoreMap    map[UserId]int
	ScoreArray    []int
	IdArray       []UserId
	NicknameArray []string
}

// Set adds a new ranking entry.
func (t *RankData) Set(id UserId, newScore int) (rank int, tieCount int) {
	return t.SetWithNickname(id, newScore, "")
}

// SetWithNickname adds a new ranking entry with a nickname.
func (t *RankData) SetWithNickname(id UserId, newScore int, nickname string) (rank int, tieCount int) {
	if oldScore, ok := t.IdScoreMap[id]; ok {
		// Existing id
		oldRank, oldTieCount := getRankAndTieCountZeroBasedDesc(&t.ScoreArray, oldScore)
		if oldScore == newScore {
			// The same score; nothing to do
			return oldRank, oldTieCount
		}
		// TODO Should change to binary search from linear search
		oldIdArrayIndex := -1
		for i := oldRank; i < oldRank+oldTieCount; i++ {
			if t.IdArray[i] == id {
				oldIdArrayIndex = i
				break
			}
		}
		if oldIdArrayIndex < 0 {
			log.Fatal("CRITICAL ERROR: oldIdArrayIndex not found!")
		} else {
			// Update to new score
			t.IdScoreMap[id] = newScore
			newRank, newTieCount := updateScoreDesc(&t.ScoreArray, oldScore, newScore)
			moveUserIdWithinSlice(&t.IdArray, oldIdArrayIndex, newRank)
			t.NicknameArray[oldIdArrayIndex] = nickname
			moveStringWithinSlice(&t.NicknameArray, oldIdArrayIndex, newRank)
			// Update nickname (if changed)
			//t.NicknameArray[newRank] = nickname
			return newRank, newTieCount
		}
		return -1, -1
	} else {
		// New id, new score
		t.IdScoreMap[id] = newScore
		rank, tieCount = insertNewScoreDesc(&t.ScoreArray, newScore)
		insertUserIdToSlice(&t.IdArray, rank, id)
		insertStringToSlice(&t.NicknameArray, rank, nickname)
		return rank, tieCount
	}
	// Unreachable code
	return -1, -1
}

// Get a single rank entry by User ID.
func (t *RankData) Get(id UserId) (score int, rank int, tieCount int, err error) {
	if oldScore, ok := t.IdScoreMap[id]; ok {
		rank, tieCount = getRankAndTieCountZeroBasedDesc(&t.ScoreArray, oldScore)
		return oldScore, rank, tieCount, nil
	}
	return -1, -1, -1, errors.New("id not exist")
}

// PrintAll prints all rank data for debugging purpose.
func (t *RankData) PrintAll() {
	rank := 0
	tieCount := 1
	for i, c := 0, len(t.IdArray); i < c; i++ {
		fmt.Printf("RankData.%v: %v %v\n", rank, t.IdArray[i], t.ScoreArray[i])
		if i < c-1 {
			if t.ScoreArray[i+1] == t.ScoreArray[i] {
				tieCount++
			} else {
				rank += tieCount
				tieCount = 1
			}
		}
	}
}

// getRankZeroBasedDesc returns rank for given score.
// descArr is the previous score array which should be sorted in
// descending order.
// It returns 0 if score is the highest score so far and
// returns len(*descArr) - 1 if score is the lowest score so far.
func getRankZeroBasedDesc(descArr *[]int, score int) (rankZeroBased int) {
	return sort.Search(len(*descArr), func(i int) bool { return score >= (*descArr)[i] })
}

// getNextRankZeroBasedDesc returns 'next' rank for given score.
// descArr is the previous score array which should be sorted in
// descending order.
func getNextRankZeroBasedDesc(descArr *[]int, score int) (nextRankZeroBased int) {
	return sort.Search(len(*descArr), func(i int) bool { return score > (*descArr)[i] })
}

// getRankAndTieCountZeroBasedDesc returns rank and tie count
// for given score.
func getRankAndTieCountZeroBasedDesc(descArr *[]int, score int) (rankZeroBased int, tieCount int) {
	rankZeroBased = getRankZeroBasedDesc(descArr, score)
	tieCount = getNextRankZeroBasedDesc(descArr, score) - rankZeroBased
	return rankZeroBased, tieCount
}

// insertScoreToSlice inserts a score x to given slice s at index i.
func insertScoreToSlice(s *[]int, i int, x int) {
	*s = append(*s, 0)
	copy((*s)[i+1:], (*s)[i:])
	(*s)[i] = x
}

// insertUserIdToSlice inserts a user ID x to given slice s at index i.
func insertUserIdToSlice(s *[]UserId, i int, x UserId) {
	*s = append(*s, UserId{})
	copy((*s)[i+1:], (*s)[i:])
	(*s)[i] = x
}

// insertStringToSlice inserts a string x to given slice s at index i.
func insertStringToSlice(s *[]string, i int, x string) {
	*s = append(*s, "")
	copy((*s)[i+1:], (*s)[i:])
	(*s)[i] = x
}

// removeUserIdFromSlice removes a user ID from given slice s.
func removeUserIdFromSlice(s *[]UserId, i int) {
	*s = append((*s)[0:i], (*s)[i+1:]...)
}

// removeStringFromSlice removes a string from given slice s.
func removeStringFromSlice(s *[]string, i int) {
	*s = append((*s)[0:i], (*s)[i+1:]...)
}

// moveUserIdWithinSlice moves an user ID element index i to j.
// Note that this function does not remove any element.
func moveUserIdWithinSlice(s *[]UserId, i, j int) {
	if i == j {
		return
	}
	m := (*s)[i]
	removeUserIdFromSlice(s, i)
	insertUserIdToSlice(s, j, m)
}

// moveStringWithinSlice moves a string element index i to j.
// Note that this function does not remove any element.
func moveStringWithinSlice(s *[]string, i, j int) {
	if i == j {
		return
	}
	m := (*s)[i]
	removeStringFromSlice(s, i)
	insertStringToSlice(s, j, m)
}

// updateScoreDesc updates an existing score entry from oldScore to newScore.
func updateScoreDesc(descArr *[]int, oldScore, newScore int) (int, int) {
	if oldScore == newScore {
		return -1, -1
	}
	oldScoreRank, oldScoreTieCount := getRankAndTieCountZeroBasedDesc(descArr, oldScore)
	if oldScore > newScore {
		descArrPre := (*descArr)[0:(oldScoreRank + oldScoreTieCount - 1)]
		descArrPost := (*descArr)[oldScoreRank+oldScoreTieCount:]
		insertRank, insertTieCount := getRankAndTieCountZeroBasedDesc(&descArrPost, newScore)
		insertScoreToSlice(&descArrPost, insertRank, newScore)
		*descArr = append(descArrPre, descArrPost...)
		return len(descArrPre) + insertRank, insertTieCount + 1
	}
	if oldScore < newScore {
		descArrPre := (*descArr)[0:oldScoreRank]
		descArrPost := (*descArr)[oldScoreRank+1:]
		insertRank, insertTieCount := getRankAndTieCountZeroBasedDesc(&descArrPre, newScore)
		insertScoreToSlice(&descArrPre, insertRank, newScore)
		*descArr = append(descArrPre, descArrPost...)
		return insertRank, insertTieCount + 1
	}
	// Unreachable code
	return -1, -1
}

// insertNewScoreDesc inserts a new score entry to descArr.
// It returns both rank and tie count for a new score entry.
func insertNewScoreDesc(descArr *[]int, newScore int) (insertRank int, insertTieCount int) {
	insertRank, insertTieCount = getRankAndTieCountZeroBasedDesc(descArr, newScore)
	insertScoreToSlice(descArr, insertRank, newScore)
	return insertRank, insertTieCount + 1
}

// newRank returns a newly allocated RankData.
func newRank() *RankData {
	return &RankData{
		IdScoreMap: make(map[UserId]int),
		ScoreArray: make([]int, 0),
		IdArray:    make([]UserId, 0),
	}
}

// RankService is a struct containing a whole data a rank service need to run.
type RankService struct {
	rank *RankData
}

// Set is a rpc call wrapper for SetWithNickname.
func (t *RankService) Set(args *shared_server.ScoreItem, reply *int) error {
	rank, _ := t.rank.SetWithNickname(args.Id, args.Score, args.Nickname)
	*reply = rank
	return nil
}

// Get is a rpc call wrapper for Get.
func (t *RankService) Get(args *[16]byte, reply *shared_server.ScoreRankItem) error {
	score, rank, _, err := t.rank.Get(*args)
	if err != nil {
		log.Printf("Get failed: %v", err)
		reply.Id = *args
		reply.Score = 1500
		reply.Rank = -1
	} else {
		reply.Id = *args
		reply.Score = score
		reply.Rank = rank
	}
	return nil
}

// GetLeaderboard is a rpc call wrapper for getting a leaderboard data.
func (t *RankService) GetLeaderboard(args *shared_server.LeaderboardRequest, reply *shared_server.LeaderboardReply) error {
	scoreCount := len(t.rank.ScoreArray)
	if scoreCount == 0 {
		log.Printf("Score empty")
		return nil
	}
	if scoreCount <= args.StartIndex || args.StartIndex < 0 {
		log.Printf("StartIndex out of bounds error")
		return nil
	}
	if args.Count <= 0 || args.Count > 100 {
		log.Printf("Count out of bounds error")
		return nil
	}

	firstId := t.rank.IdArray[args.StartIndex]
	_, rank, tieCount, err := t.rank.Get(firstId)
	if err != nil {
		log.Printf("Get error")
	} else {
		reply.FirstItemRank = rank
		reply.FirstItemTieCount = tieCount
		items := make([]shared_server.LeaderboardItem, 0)
		c := args.Count
		if args.StartIndex+c > scoreCount {
			c = scoreCount - args.StartIndex
		}
		for i := 0; i < c; i++ {
			items = append(items, shared_server.LeaderboardItem{
				Nickname: t.rank.NicknameArray[args.StartIndex+i],
				Score:    t.rank.ScoreArray[args.StartIndex+i],
			})
		}
		reply.Items = items
	}
	return nil
}

// main is an entry function for this package.
func main() {
	log.SetFlags(log.Lshortfile | log.LstdFlags)
	//selfTest()
	server := rpc.NewServer()
	rankService := &RankService{
		newRank(),
	}
	server.RegisterName("RankService", rankService)
	addr := ":20172"
	log.Printf("Listening %v for rank service...", addr)
	// Listen for incoming tcp packets on specified port.
	l, e := net.Listen("tcp", addr)
	if e != nil {
		log.Fatal("listen error:", e)
	}
	server.Accept(l)
}
