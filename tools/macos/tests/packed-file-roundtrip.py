import os
import Blender
from Blender import Image

mode = os.environ['PACKED_FILE_ROUNDTRIP_MODE']
image_path = os.environ['PACKED_FILE_ROUNDTRIP_IMAGE']
blend_path = os.environ['PACKED_FILE_ROUNDTRIP_BLEND']
image_name = os.path.basename(image_path)

if mode == 'create':
	image = Image.Load(image_path)
	assert image is not None
	image.pack()
	Blender.Save(blend_path, 1)
	print 'PACKED_FILE_CREATE_PASS'
elif mode == 'verify':
	image = Image.Get(image_name)
	assert image is not None
	assert image.packed
	print 'PACKED_FILE_REOPEN_PASS'
else:
	raise RuntimeError('unknown packed-file round-trip mode: ' + mode)
