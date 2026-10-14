# Native dependency source research

Research date: 2026-10-13. Scope: source provenance and compatibility seams for the native build. These findings do not prove a working dependency build, application launch, physical CD playback, or reference parity.

## Cocoa GHOST donor

Use Blender **2.50a1** as the closest **verified release archive containing Cocoa GHOST**. The inspected 2.49b archive contains no Cocoa backend. The 2.50a1 archive contains `GHOST_SystemCocoa.{h,mm}`, `GHOST_WindowCocoa.{h,mm}`, and `GHOST_DisplayManagerCocoa.{h,mm}`. This is an alpha-era donor, not evidence of modern ARM compatibility. Sources: [official source archive index](https://download.blender.org/source/), [2.50a1 archive](https://mirror.blender.org/source/blender-2.50a1.tar.gz), [2.49b archive](https://mirror.blender.org/source/blender-2.49b.tar.gz).

Downloaded archive hashes, measured during this research:

| Source | SHA256 | Published MD5, matched locally |
| --- | --- | --- |
| `blender-2.50a1.tar.gz` | `99477f0b355c12a11decf0a01f6fe007a91860c2a66a7ef1f86825575d0133b2` | `77d1b3d807a861b47bd5bec7966928d5` |
| `blender-2.49b.tar.gz` | `23554db4aa10b00e0e760a8bf9c4a9f749670d815c8bc874a04746adc4752488` | `718eef6a4ff377989e829b92a886cc5b` |

Published checksum files: [2.50a1 MD5](https://mirror.blender.org/source/blender-2.50a1.tar.gz.md5sum), [2.49b MD5](https://mirror.blender.org/source/blender-2.49b.tar.gz.md5sum). SHA256 values above are local measurements, not claimed upstream signatures.

Keep the 2.45 public GHOST contract; adapt the donor's platform implementation. Comparing the two source trees identifies these seams:

| Seam | 2.50a1 change relative to this repository | Adaptation consequence |
| --- | --- | --- |
| `GHOST_ISystem::createWindow` | Adds `numOfAASamples` and `parentWindow` after `stereoVisual`. | Preserve the existing 2.45 signature; choose corresponding donor defaults internally. |
| `GHOST_IWindow` | Adds OS-window handle, accepted drag operation, modified state, and cursor grab methods. | Cocoa implementation depends on new base-class state and methods; copying only six platform files will not compile unchanged. |
| `GHOST_ISystem` | Adds NDOF, clipboard, and system/user directory methods. | Do not require unrelated callers to adopt a newer GHOST API to obtain native windows. |
| Events | Uses `GHOST_EventNDOF`, `GHOST_EventTrackpad`, `GHOST_EventDragnDrop`, and `GHOST_EventString`. | 2.45 does not consume these newer event families. Preserve its key/button/wheel/cursor/window events; explicitly map any retained native callbacks. |
| Window lookup | Calls `getWindowAssociatedWithOSWindow`. | Older window-manager implementation lacks that donor helper. Supply a local association or adapt the lookup. |
| Rendering | Uses `NSOpenGLView` and `NSOpenGLContext`, including shared contexts. | Native Cocoa/OpenGL implementation replaces AGL; retain the legacy OpenGL profile required by the renderer. |
| Ownership | Uses manual retain/release and autorelease pools. | Build as Objective-C++ without ARC unless ownership is deliberately converted and checked. |

These claims come from the corresponding files in the hash-verified [2.50a1 source archive](https://mirror.blender.org/source/blender-2.50a1.tar.gz), compared with `intern/ghost/GHOST_ISystem.h`, `GHOST_IWindow.h`, and the existing backend/base classes in this repository. The donor's `GHOST_EventKey` constructor is unchanged apart from licensing/comment metadata, which makes the keyboard event seam relatively small.

Donor limitations require explicit follow-up: `GHOST_DisplayManagerCocoa::getNumDisplaySettings` reports three settings while `getDisplaySetting` returns only the current visible-frame dimensions; `setCurrentDisplaySetting` returns failure unconditionally. Therefore wholesale adoption would not establish display-mode/fullscreen parity. The window source conditionally includes Carbon for pre-10.6 `SetSystemUIMode`, while newer SDK branches use Cocoa presentation options. Preserve its GPL notices and contributor attribution when importing or adapting it. Source: [2.50a1 archive](https://mirror.blender.org/source/blender-2.50a1.tar.gz), `GHOST_DisplayManagerCocoa.mm:54-168`, `GHOST_WindowCocoa.mm:30-35` and fullscreen branches.

## Python 2.5.6

**Python 2.5.6** is the final source-only Python 2.5 release. Prefer its [bzip2 source archive](https://www.python.org/ftp/python/2.5.6/Python-2.5.6.tar.bz2), retaining the Python 2.5 C API and standard library. The [official release page](https://www.python.org/downloads/release/python-256/) publishes MD5 `5d45979c5f30fb2dd5f067c6b06b88e4`; the downloaded archive matches. Measured SHA256: `57e04484de051decd4741fb4a4a3f543becc9a219af8b8063b5541e270f26dcc`.

The downloaded source's `configure.in` assumes PPC/i386 for universal SDK builds, defaults Darwin extension deployment logic to 10.3, and adds `-lSystemStubs`/`arch` to framework libtool commands. Its `setup.py` attempts Carbon toolbox extensions by default. A private, non-framework static `libpython2.5.a` build avoids the framework and obsolete universal-architecture paths; it still needs actual modern compiler checks and successful interpreter/extension builds. Source: [Python 2.5.6 archive](https://www.python.org/ftp/python/2.5.6/Python-2.5.6.tar.bz2), `configure.in` framework/universal/Darwin sections and `setup.py` platform module detection.

`--disable-toolbox-glue` is an upstream configure switch recognized by `setup.py`. It excludes Python's obsolete Carbon glue, not Blender's Python API, but is a compatibility limitation requiring documentation and script-import checks. A repository search found no `Carbon`, `MacOS`, `EasyDialogs`, `Nav`, `Audio_mac`, or `audiodev` imports under `release/scripts`; that narrow search does not prove all Python standard-library or third-party features work. On a case-insensitive Mac filesystem, the source directory `Python/` can make `make python` select a directory rather than the actual executable target; inspect generated `Makefile` and use its configured executable target.

## SDL 1.2 audio and CD APIs

Upstream **sdl12-compat 1.2.68** provides SDL 1.2 source/binary APIs using SDL2, and explicitly documents arm64/x86_64 macOS builds. Its pinned [README](https://raw.githubusercontent.com/libsdl-org/sdl12-compat/release-1.2.68/README.md) requires SDL2 headers at build time and SDL2 at runtime. Its pinned [CMake source](https://raw.githubusercontent.com/libsdl-org/sdl12-compat/release-1.2.68/CMakeLists.txt) builds `libSDL-1.2.0.dylib` on macOS; static compatibility builds are limited to Linux. Reuse an installed native SDL2 where possible; package and verify the runtime SDL2 resolution as well as the compatibility dylib.

Pinned source download candidate: [release-1.2.68 source archive](https://github.com/libsdl-org/sdl12-compat/archive/refs/tags/release-1.2.68.tar.gz). The [upstream release metadata](https://api.github.com/repos/libsdl-org/sdl12-compat/releases/tags/release-1.2.68) exposes Windows binary assets but no release source-archive digest. This source tarball was not downloaded or hash-verified during the research; compute and record its SHA256 before relying on it in a reproducible build. The guessed `libsdl.org/release/sdl12-compat-1.2.68.tar.gz` URL returns 404.

The pinned README's `SDL12COMPAT_FAKE_CDROM_PATH` exposes a directory of `trackXX.mp3` files through CD-audio APIs. This can keep the legacy `SDL_CD*` calls linkable but **does not establish physical CD-drive parity**. `intern/SoundSystem/sdl/SND_SDLCDDevice.cpp` uses drive enumeration, track playback, pause/resume, and status; `source/blender/src/seqaudio.c` uses `SDL_OpenAudio`. Keep these call sites and separately validate real audio playback and CD behavior. Source: [sdl12-compat 1.2.68 README](https://raw.githubusercontent.com/libsdl-org/sdl12-compat/release-1.2.68/README.md), fake CD-ROM option and build requirements; repository call sites above.

The compatibility layer is a practical native build route, with the physical-CD gap retained as an open requirement. Direct upstream [SDL 1.2 Classic](https://github.com/libsdl-org/SDL-1.2) remains available, but its historical macOS drivers are not evidence of current ARM support. Do not label a successfully linked compatibility build as full SDL or original-release behavior parity.
