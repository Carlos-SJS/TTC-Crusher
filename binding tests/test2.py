import cppimport.import_hook
import Player

cfg = {}
cfg['dependencies'] = ['file1.h', 'file2.h']

p = Player.Player(5)
print(p.getX())
p.setX(2)
print(p.getX())