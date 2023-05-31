from round_robin import RoundRobin
from player_random import TTCPlayer
from TTC_Crusher import TTC_Crusher_Player

def print_winner():
    with open('results/leaderboard.txt', 'r') as f:
        l = f.readlines()
        
        winner = l[1].replace('(', '').replace(',', '').replace('\'', '').split(' ')[1]
        off = 0
        
        if winner == "Gennedy":
            off = 1
        
        wscore = l[1].replace('(', '').replace(',', '').replace('\'', '').split(' ')[3+off]
        lscore = l[2].replace('(', '').replace(',', '').replace('\'', '').split(' ')[3+(off^1)]
        
        print(f"The winner is: {winner}")
        print(f"{wscore} - {lscore}")

player1 =TTCPlayer("juan")
player2 = TTC_Crusher_Player("Gennedy Korotkevich")
#player3 = TTCPlayer("juan3")
#player4 = TTCPlayer("juan4")

if __name__ == '__main__':
    tournament = RoundRobin([player1, player2], 20, 5, 70)
    tournament.start()
    
    print_winner()