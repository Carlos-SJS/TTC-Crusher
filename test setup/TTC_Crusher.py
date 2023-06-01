import PlayerCore
import time

class TTC_Crusher_Player:
    def __init__(self, name):
        self.name = name
        
        self.core = PlayerCore.PlayerCore()
        self.color = 0

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
        
        new_world = self.core.getMove(world)
        
        if self.color == 1:
            world = [ [x*-1 for x in l] for l in new_world]
            return world
        
        print("Time taken: ", time.time() - start)

        return new_world

    def reset(self):
        # Do nothing, we reset in the setColor call
        pass