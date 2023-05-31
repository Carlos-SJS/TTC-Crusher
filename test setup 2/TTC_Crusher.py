import PlayerCore
import time

class TTC_Crusher_Player:
    def __init__(self, name):
        self.name = name
        
        self.core = PlayerCore.PlayerCore()
        self.color = 0
    
    def __printBoard(self, board):
        print('-------')
        for i in range(len(board)):
            for j in range(len(board)):
                if board[i][j] >= 0:
                    print(' ', end='')
                print(board[i][j], end='')
            print()
        print('-------')


    def setColor(self, color):
        if(color == 1):
            color = 0
        else:
            color = 1
            
        self.color = color
            
        self.core.reset(color)

    def play(self, world):
        start = time.time()
        
        if self.color == 1:
            world = [ [x*-1 for x in l] for l in world]
            
        #for l in world:
        #    print(l)
        
        new_world = self.core.getMove(world)
        
        #for l in new_world:
        #    print(l)
        
        self.__printBoard(world)
        self.__printBoard(new_world)
        
        print("Time taken D: ", time.time() - start)
        
        if self.color == 1:
            world = [ [x*-1 for x in l] for l in new_world]
            return world

        return new_world

    def reset(self):
        # Do nothing, we reset in the setColor call
        pass