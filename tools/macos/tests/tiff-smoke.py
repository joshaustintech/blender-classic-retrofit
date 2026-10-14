# Run with Blender -b preview.blend -P tiff-smoke.py.
import os, tempfile
from Blender import Scene, Image
from Blender.Scene import Render
folder = tempfile.mkdtemp(prefix='blender245-tiff-')
try:
    context = Scene.GetCurrent().getRenderingContext()
    context.setImageType(Render.TIFF)
    context.setRenderPath(folder + '/')
    context.render()
    names = [name for name in os.listdir(folder) if name.endswith('.tif')]
    assert len(names) == 1, names
    path = os.path.join(folder, names[0])
    assert os.path.isfile(path), path
    image = Image.Load(path)
    assert image.getSize()[0] > 0
    print 'NATIVE_TIFF_PASS: packaged TIFF write and read'
finally:
    for name in os.listdir(folder):
        os.remove(os.path.join(folder, name))
    os.rmdir(folder)
