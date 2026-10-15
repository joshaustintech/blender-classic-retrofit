# Run through Blender's legacy CLI: blender -b scene.blend -P python-smoke.py
import sys
import os, math, zlib, struct, xml.parsers.expat
import Blender
from Blender import BGL, Mathutils, Scene
assert sys.version_info[:2] == (2, 5)
assert Blender.Get('version') == 245
assert zlib.decompress(zlib.compress('native')) == 'native'
assert math.sqrt(9) == 3
if sys.platform == 'darwin' and '.app/Contents/MacOS/' in sys.executable:
    assert '/Contents/Resources/python' in os.__file__, os.__file__

vector = Mathutils.Vector(1.0, 2.0, 3.0)
assert len(vector) == 3
assert vector[-1] == 3.0
vector[1] = 5.0
assert list(vector[:2]) == [1.0, 5.0]
assert len(vector[:1 << 40]) == 3
for index in [1 << 40, -(1 << 40)]:
    try:
        vector[index]
    except IndexError:
        pass
    else:
        raise AssertionError('Vector index truncated to 32 bits')

buffer = BGL.Buffer(BGL.GL_FLOAT, [3], [1.0, 2.0, 3.0])
assert len(buffer) == 3
buffer[1] = 7.0
assert buffer[1] == 7.0
assert buffer[-1] == 3.0
for index in [1 << 40, -(1 << 40)]:
    try:
        buffer[index]
    except IndexError:
        pass
    else:
        raise AssertionError('BGL index truncated to 32 bits')
    try:
        buffer[index] = 9.0
    except IndexError:
        pass
    else:
        raise AssertionError('BGL assignment index truncated to 32 bits')

assert len(Mathutils.Euler(1.0, 2.0, 3.0)) == 3
assert len(Mathutils.Quaternion(1.0, 0.0, 0.0, 0.0)) == 4
matrix = Mathutils.Matrix([1, 0, 0], [0, 1, 0], [0, 0, 1])
assert len(matrix) == 9

quat_a = Mathutils.Quaternion(1.0, 2.0, 3.0, 4.0)
quat_b = Mathutils.Quaternion(5.0, 6.0, 7.0, 8.0)
assert abs(quat_a * quat_b - 70.0) < 1e-6
assert abs(quat_a * quat_a - 30.0) < 1e-6
assert list(quat_a * 2) == [2.0, 4.0, 6.0, 8.0]
assert list(2 * quat_a) == [2.0, 4.0, 6.0, 8.0]
rotated = Mathutils.Quaternion(1.0, 0.0, 0.0, 0.0) * Mathutils.Vector(1.0, 2.0, 3.0)
assert list(rotated) == [1.0, 2.0, 3.0]
try:
    quat_a * Mathutils.Vector(1.0, 2.0)
except TypeError:
    pass
else:
    raise AssertionError('Quaternion multiplication accepted a 2D vector')
try:
    quat_a * 'invalid'
except TypeError:
    pass
else:
    raise AssertionError('Quaternion multiplication accepted a string')

scene = Scene.GetCurrent()
assert len(scene.objects) > 0
scene.properties['native_size_check'] = [1, 2, 3]
assert len(scene.properties['native_size_check']) == 3
assert scene.properties['native_size_check'][2] == 3
print 'NATIVE_PYTHON_PASS: legacy API, sequence sizes, wide-index rejection, ID properties'
