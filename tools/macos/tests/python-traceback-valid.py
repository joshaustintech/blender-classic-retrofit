import Blender
from Blender import Object

objects = Object.Get()
assert objects
assert objects[0].getName()
assert objects[0].users > 0
print 'TICKET04_VALID_SCRIPT'
