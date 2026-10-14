# Blender 2.45 host OpenGL capability probe

Source: `docs/evidence/legacy-opengl-probe.mm`

The original probe was compiled in a temporary directory. From the repository root, choose a disposable executable path as `$RETROFIT_PROBE`; the equivalent compile command is:

```sh
xcrun clang++ -arch arm64 -std=c++11 -fobjc-arc -Wno-deprecated-declarations docs/evidence/legacy-opengl-probe.mm -framework Cocoa -framework OpenGL -o "$RETROFIT_PROBE"
```

Run command:

```sh
"$RETROFIT_PROBE"
```

Sandbox run output (program exit 1):

```text
pixel_format=nil
```

Identical executable outside sandbox (approved escalation; exit 0):

```text
pixel_format=created
context=created
version=2.1 Metal - 91.7
renderer=Apple M1 Pro
vendor=Apple
framebuffer_status=0x8cd5
clear_rgba=17,34,51,255
fixed_function_rgba=204,102,51,255
gl_error=0x0
result=PASS
```

`file "$RETROFIT_PROBE"` reports `Mach-O 64-bit executable arm64`.

Probe created no GUI windows. It created a Cocoa NSOpenGL context using NSOpenGLProfileVersionLegacy, allocated a 4x4 RGBA8 texture attached to an EXT framebuffer, read a known clear color, drew an immediate-mode quad using projection/modelview matrices, and read its known color.

This proves only host offscreen legacy OpenGL/fixed-function capability under the current environment. It does not prove Blender builds, onscreen swap/drawing, picking, font rendering, every extension, window/input behavior, or Blender Internal output parity. The sandbox failure is access-dependent and must not be interpreted as host graphics incompatibility.

## Packaging observations

Official archive scripts compared against repository `release/scripts/` by relative name and SHA256:

```text
archive_script_files 147
repo_script_files 135
common_files 135
common_byte_identical 106
archive_only_count 12
repo_only_count 0
```

Archive-only files:

```text
bpymodules/colladaImEx/__init__.py
bpymodules/colladaImEx/collada.py
bpymodules/colladaImEx/cstartup.py
bpymodules/colladaImEx/cutils.py
bpymodules/colladaImEx/helperObjects.py
bpymodules/colladaImEx/logo.png
bpymodules/colladaImEx/translator.py
bpymodules/colladaImEx/xmlUtils.py
colladaExport14.py
colladaImport14.py
collada_export.py
collada_import.py
```

The 29 shared files with differing bytes have not been semantically reviewed; differences alone do not establish changed behavior.

Repository embeds default startup data in `source/blender/src/B.blend.c` (datatoc_B_blend_size=98212); `source/blender/src/usiblender.c:498-507` loads user `.B.blend` or embedded fallback. Repository embeds font bytes in `source/blender/src/bfont.ttf.c` (datatoc_bfont_ttf_size=191532); `source/blender/src/language.c:245-253` uses them for default/fallback fonts. Thus missing standalone `.B.blend` or font files in the original archive do not imply missing original startup/font behavior. Embedded assets need parity verification independently of bundle member names.

Current Apple CMake staging (`source/creator/CMakeLists.txt:112-117`) references `bin/.blender/locale`, which is absent in this checkout, copies legacy `.bfont.ttf`/`.Blanguages`, and copies repository scripts. Official archive puts locale under `Contents/Resources/locale` and ships the 147-file script inventory. Packaging acceptance must compare intended resource contents and runtime discovery, prove clean-user startup/default font, test bundled script registration/imports including Collada, and test locale switching. A successfully linked executable does not cover those checks.
