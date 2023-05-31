from round_robin import RoundRobin
from player_random import TTCPlayer



player1 =TTCPlayer("juan1")
player2 = TTCPlayer("juan2")
#player3 = TTCPlayer("juan3")
#player4 = TTCPlayer("juan4")

if __name__ == '__main__':
    tournament = RoundRobin([player1, player2], 30, 5, 150)
    tournament.start()