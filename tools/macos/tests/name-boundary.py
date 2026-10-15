# Run through Blender's legacy CLI in create or verify mode.
import os
import Blender

filepath = os.environ['BLENDER_NAME_BOUNDARY_FILE']
verify = os.environ.get('BLENDER_NAME_BOUNDARY_VERIFY') == '1'

if verify:
    expected = ['abcdefghijklmnopqrst', 'abcdefghijklmnopqrstu',
                'abcdefghijklmnopq', 'abcdefghijklmnopq.001',
                '\xc3\xa9' * 9 + 'ab', 'normal', 'normal.001', '', '.001']
    for index, name in enumerate(expected):
        ob = Blender.Object.Get(name)
        assert ob.properties['ticket02_identity'] == index, (name, ob.properties)
    print 'NATIVE_NAME_BOUNDARY_PASS: reopened names'
else:
    names = ['abcdefghijklmnopqrst', 'abcdefghijklmnopqrstu',
             'abcdefghijklmnopqrstuv', 'abcdefghijklmnopqrstu',
             '\xc3\xa9' * 9 + 'ab', 'normal', 'normal', '', '']
    scene = Blender.Scene.GetCurrent()
    objects = [Blender.Object.New('Empty', name) for name in names]
    for ob in objects:
        scene.link(ob)
    actual = [ob.name for ob in objects]
    for index, ob in enumerate(objects):
        ob.properties['ticket02_identity'] = index
    assert actual == ['abcdefghijklmnopqrst', 'abcdefghijklmnopqrstu',
                      'abcdefghijklmnopq', 'abcdefghijklmnopq.001',
                      '\xc3\xa9' * 9 + 'ab', 'normal', 'normal.001', '', '.001'], actual
    Blender.Save(filepath, 1)
    print 'NATIVE_NAME_BOUNDARY_PASS: created names'
