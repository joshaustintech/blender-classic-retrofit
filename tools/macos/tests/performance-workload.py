"""Repeatable Blender 2.45 Python workload oracle for performance baselines.

Run with Blender's embedded Python 2.5 and PERF_MODE set to one of:
text-read, text-edit, image-io, image-pixels, geometry, render.
"""
import hashlib
import os
import struct
import time

import Blender
from Blender import Image, Mathutils, Mesh, Text


ROOT = os.environ.get('PERF_ROOT', os.getcwd())
TMP = os.environ.get('PERF_TMP', '/private/tmp')
BLENDER = os.environ.get('PERF_BLENDER')
SCENE = os.environ.get('PERF_SCENE', os.path.join(ROOT, 'release/datafiles/preview.blend'))
TEXT_PATH = os.path.join(TMP, 'ticket09-text.txt')
IMAGE_PATH = os.path.join(ROOT, 'tools/macos/tests/fixtures/performance-2048.png')


def report(name, values):
    ordered = sorted(values)
    middle = len(ordered) / 2
    median = (ordered[middle] + ordered[middle - 1]) / 2.0 if len(ordered) % 2 == 0 else ordered[middle]
    p95 = ordered[int(len(ordered) * 0.95 + 0.999999) - 1]
    print 'PERF_RESULT %s samples=%d median_s=%.6f p95_s=%.6f min_s=%.6f max_s=%.6f' % (
        name, len(ordered), median, p95, ordered[0], ordered[-1])


mode = os.environ.get('PERF_MODE', 'text-read')
if mode == 'text-read':
    text = Text.Load(TEXT_PATH)
    expected = 'line-000000-name-and-value=0123456789abcdef'
    elapsed = []
    digest = None
    for unused in range(12):
        started = time.clock()
        lines = text.asLines()
        elapsed.append(time.clock() - started)
        assert len(lines) == 50000
        assert lines[0] == expected
        assert lines[-1] == 'line-049999-name-and-value=0123456789abcdef'
        digest = hashlib.sha256('\n'.join(lines)).hexdigest()
        assert digest == '3b56fb5a1a356e1e88054e06712ea09f3cbac3bac1893275af66b0109a13c63a'
    print 'PERF_ORACLE text-lines=50000 sha256=%s' % digest
    report(mode, elapsed)
elif mode == 'text-edit':
    payload = open(TEXT_PATH, 'rb').read()
    text = Text.New('perf-text-edit')
    elapsed = []
    for unused in range(10):
        text.clear()
        started = time.clock()
        text.write(payload)
        elapsed.append(time.clock() - started)
        lines = text.asLines()
        assert len(lines) == 50000
        assert lines[0] == 'line-000000-name-and-value=0123456789abcdef'
        assert lines[-1] == 'line-049999-name-and-value=0123456789abcdef'
    print 'PERF_ORACLE text-edit bytes=%d lines=50000' % len(payload)
    report(mode, elapsed)
elif mode == 'image-io':
    elapsed = []
    outputs = []
    for index in range(1, 9):
        path = os.path.join(TMP, 'ticket09-image-%d.png' % index)
        started = time.clock()
        image = Image.Load(path)
        assert image.getSize() == [2048, 2048]
        image.save()
        elapsed.append(time.clock() - started)
        outputs.append(hashlib.sha256(open(path, 'rb').read()).hexdigest())
    assert len(set(outputs)) == 1
    assert outputs[0] == '10c685ddc76b4bba232c0490899485a501c037d5c9dbbdce11b61a144a434866'
    print 'PERF_ORACLE image-2048x2048 sha256=%s' % outputs[0]
    report(mode, elapsed)
elif mode == 'image-pixels':
    image = Image.Load(IMAGE_PATH)
    width, height = image.getSize()
    elapsed = []
    checksums = []
    for unused in range(100):
        started = time.clock()
        total = 0
        for y in range(0, height, 16):
            for x in range(0, width, 16):
                rgba = image.getPixelI(x, y)
                total += rgba[0] + 3 * rgba[1] + 5 * rgba[2] + 7 * rgba[3]
        elapsed.append(time.clock() - started)
        checksums.append(total)
    assert len(set(checksums)) == 1
    print 'PERF_ORACLE image-samples grid=128x128 checksum=%d' % checksums[0]
    report(mode, elapsed)
elif mode == 'geometry':
    side = 128
    coords = []
    faces = []
    for y in range(side):
        for x in range(side):
            coords.append((float(x), float(y), float((x * y) % 17)))
    for y in range(side - 1):
        for x in range(side - 1):
            index = y * side + x
            faces.append((index, index + 1, index + side + 1, index + side))
    mesh = Mesh.New('perf-grid')
    mesh.verts.extend(coords)
    mesh.faces.extend(faces)
    assert len(mesh.verts) == side * side
    assert len(mesh.faces) == (side - 1) * (side - 1)
    matrix = Mathutils.ScaleMatrix(1.0001, 4, Mathutils.Vector(1, 0, 0))
    elapsed = []
    for unused in range(1200):
        started = time.clock()
        mesh.transform(matrix, 1)
        elapsed.append(time.clock() - started)
    first = tuple(mesh.verts[0].co[:])
    last = tuple(mesh.verts[-1].co[:])
    digest = hashlib.sha256(repr((first, last, len(mesh.verts), len(mesh.faces))).encode('ascii')).hexdigest()
    assert digest == '9749f96ce8032a6c3e511374d816ded2e4bca711047b75cda49e5b73e52718d8'
    print 'PERF_ORACLE geometry verts=%d faces=%d sha256=%s' % (len(mesh.verts), len(mesh.faces), digest)
    report(mode, elapsed)
elif mode == 'render':
    import subprocess
    elapsed = []
    outputs = []
    if not BLENDER:
        raise RuntimeError('PERF_BLENDER must name the optimized Blender executable')
    for index in range(8):
        base = os.path.join(TMP, 'ticket09-render-%02d' % index)
        started = time.time()
        subprocess.check_call([BLENDER, '-b', SCENE, '-t', '1', '-o', base, '-F', 'PNG', '-f', '1'])
        elapsed.append(time.time() - started)
        rendered_path = base + '0001.png'
        rendered = open(rendered_path, 'rb').read()
        assert rendered[:8] == '\x89PNG\r\n\x1a\n'
        assert struct.unpack('>II', rendered[16:24]) == (150, 150)
        outputs.append(hashlib.sha256(rendered).hexdigest())
    assert len(set(outputs)) == 1
    assert outputs[0] == 'bdae612d5ed0d4f9384ab8b1aab50d135e12995fd5855cbe343f2351bb8dd9df'
    print 'PERF_ORACLE render-scene=%s frames=8 resolution=150x150 OSA=5 threads=1 sha256=%s' % (
        hashlib.sha256(open(SCENE, 'rb').read()).hexdigest(), outputs[0])
    report(mode, elapsed)
else:
    raise RuntimeError('unknown PERF_MODE: %r' % mode)
