import Blender
import sys
from Blender import Object

if getattr(sys, 'last_traceback', None) is None:
    print 'TICKET04_TRACEBACK_CLEARED'
else:
    print 'TICKET04_TRACEBACK_RETAINED'

if (getattr(sys, 'last_type', None) == RuntimeError and
        str(getattr(sys, 'last_value', '')) == 'TICKET04_EXPECTED_SCRIPT_ERROR'):
    print 'TICKET04_EXCEPTION_STATE_PRESERVED'
else:
    print 'TICKET04_EXCEPTION_STATE_LOST'

objects = Object.Get()
assert objects
assert objects[0].getName()
assert objects[0].users > 0
print 'TICKET04_CURRENT_OBJECT_VALID'
