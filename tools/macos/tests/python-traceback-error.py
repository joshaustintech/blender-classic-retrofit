import Blender
from Blender import Object

objects = Object.Get()
assert objects
held_object = objects[0]
assert held_object.getName()
assert held_object.users > 0
print 'TICKET04_WRAPPED_OBJECT'
raise RuntimeError('TICKET04_EXPECTED_SCRIPT_ERROR')
