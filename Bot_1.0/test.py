import PlayerCore

player = PlayerCore.PlayerCore()

board = [
    [0,0,0,-1],
    [2,0,0,0],
    [0,0,4,0],
    [1,0,0,0],
]

new_board = player.getMove(board)

for line in new_board:
    print(line)