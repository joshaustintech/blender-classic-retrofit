import os
import Blender
from Blender import Camera, Curve, Scene, Text3d

blend_path = os.environ['FONT_ERROR_BLEND']
mode = os.environ['FONT_ERROR_MODE']

if mode == 'create':
	text = Text3d.New('Ticket03Text')
	text.setText('Blender 2.45')
	font = Text3d.Font.Load(os.path.join(os.environ['FONT_ERROR_SOURCE'], 'release/datafiles/bfont.ttf'))
	assert font is not None
	text.setFont(font)
	scene = Scene.GetCurrent()
	obj = scene.objects.new(text, 'Ticket03TextObject')
	camera_data = Camera.New('ortho', 'Ticket03CameraData')
	camera_data.scale = 12.0
	camera = scene.objects.new(camera_data, 'Ticket03Camera')
	camera.setLocation(0.0, 0.0, 8.0)
	scene.objects.camera = camera
	render = scene.getRenderingContext()
	render.sizeX = 640
	render.sizeY = 240
	render.OSALevel = 5
	Blender.Save(blend_path, 1)
	print 'FONT_ERROR_CREATE_PASS'
elif mode == 'verify':
	obj = Blender.Object.Get('Ticket03TextObject')
	assert obj is not None
	assert obj.getData().getText() == 'Blender 2.45'
	assert obj.getData().getFont().filename.endswith('bfont.ttf')
	obj.makeDisplayList()
	geometry = Curve.Get('Ticket03Text')
	assert geometry.getNumCurves() == 17
	print 'FONT_ERROR_REOPEN_PASS'
else:
	raise RuntimeError('unknown font round-trip mode: ' + mode)
