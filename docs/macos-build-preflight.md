# macOS Apple Silicon Build Preflight

## Scope and invariants

Retrofit Blender 2.45 for native Apple Silicon execution while preserving every existing feature and the UX. The first milestone is a reproducible build on the developer's macOS environment. Build success alone does not establish runtime or UX equivalence.

Preserve scripting behavior, game engine, rendering behavior, shortcuts, window interactions, saved-file compatibility, and runtime assets. Any temporary diagnostic feature omissions must be reported and cannot satisfy milestone acceptance.

Later work may use safe, standard-library-only Rust where feasible. Assess the integration boundary before adopting Rust; require output equivalence and measured benefit. Identify later Blender performance and stability fixes through their upstream changes, then reproduce their relevance here before backporting. These changes follow the working baseline.

## Verified baseline

- Source commit: `758a5a10e3a088eed05ebacf4e527b6c626a532f`, tagged `v2.45`.
- `source/blender/blenkernel/BKE_blender.h`: version `245`, subversion `0`. The imported commit message names `branches/2-44-stable`; the source and tag establish the actual version.
- Host: `arm64`, macOS `27.0.1` (`26A434`).
- Xcode developer directory: `/Applications/Xcode.app/Contents/Developer`.
- Apple Clang `21.0.0`; CMake `4.3.2`; Rust `1.97.1`.

## Executed checks

From the repository root, with a disposable directory chosen as `$RETROFIT_BUILD`:

```sh
cmake -S . -B "$RETROFIT_BUILD" -DCMAKE_OSX_ARCHITECTURES=arm64
```

Failed at `CMakeLists.txt:48`: CMake 4 has removed compatibility with the requested CMake 2.4 minimum.

```sh
cmake -S . -B "$RETROFIT_BUILD" \
  -DCMAKE_OSX_ARCHITECTURES=arm64 \
  -DCMAKE_POLICY_VERSION_MINIMUM=3.5
cmake --build "$RETROFIT_BUILD" --parallel 2
```

The policy override allowed configuration and generation, with warnings. Compilation failed: `GHOST_DisplayManager.cpp:157` has C++ list-initialization narrowing errors; `SND_SDLCDDevice.cpp:47` cannot find `SDL.h`. The `bf_string` target built. No Blender executable was produced or launched.

An independent SDK check also failed:

```sh
printf '#include <AGL/agl.h>\n' | clang -x c -fsyntax-only -
```

Result: `'AGL/agl.h' file not found`.

Reconfiguring the disposable build with `-DCMAKE_CXX_STANDARD=98` and building only `bf_ghost` passed the earlier narrowing location. It then failed on extra constructor/destructor qualification in `GHOST_SystemCarbon.h` and the missing AGL header in `GHOST_WindowCarbon.h`. This was a diagnostic override; source build settings remain unchanged.

## Remaining build work

1. Establish modern CMake settings and compatible language standards without masking errors.
2. Replace obsolete Apple library paths with verified arm64 dependencies. Current configuration falls back to PowerPC libraries and mixes detected Python 3.14 headers with a Python 2.3 library path. Preserve the legacy Python API and scripts.
3. Provide a compatible macOS GHOST backend: the existing Apple selection uses Carbon and AGL. Preserve the existing GHOST interface and application interaction semantics.
4. Audit remaining platform dependencies, including QuickTime and SDL audio/CD support, and report any unresolved feature parity.
5. Build and link the full application, package its scripts/assets, launch it, and verify representative input, save/reopen, scripting, game-engine, and rendering workflows.

Project issues and specs use local Markdown under `.scratch/<feature>/`, with the five default triage roles. Agent-skill configuration is recorded in `docs/agents/`; GitHub Issues are not the project's tracker.

## Alpha release receipt — 2026-10-14

The current tree supersedes the earlier build failures above for native compilation. It includes a Cocoa GHOST backend and packages non-system runtime libraries inside the app bundle.

Build from the repository root with a prepared dependency prefix:

```sh
cmake -S . -B ../blender245-release \
  -DCMAKE_POLICY_VERSION_MINIMUM=3.5 \
  -DCMAKE_OSX_ARCHITECTURES=arm64 \
  -DCMAKE_BUILD_TYPE=Release \
  -DWITH_GAMEENGINE=OFF \
  -DWITH_PLAYER=OFF \
  -DBLENDER_DEPENDENCY_PREFIX=/absolute/path/to/dependencies/prefix \
  -DYESIAMSTUPID=ON -DBUILD_TESTING=OFF
cmake --build ../blender245-release --target blender --parallel 6
```

Executed on Apple Silicon, macOS 27.0.1, Apple Clang 21.0.0:

- Release configuration completed with `CMAKE_OSX_ARCHITECTURES=arm64`, `WITH_GAMEENGINE=OFF`, and `WITH_PLAYER=OFF`.
- The resulting executable is Mach-O arm64. `otool -L` shows Apple system libraries/frameworks and the app-bundled SDL compatibility library; TIFF and its non-system dependencies are bundled in `Contents/MacOS`. Python 2.5.6, FreeType, JPEG, PNG, and gettext are statically linked.
- The packaged executable reported Blender 2.45 with `--version`, passed `tools/macos/tests/python-smoke.py` using `release/datafiles/preview.blend`, and rendered frame 1 to PNG.
- The Blender fullscreen toggle, previously compiled out, is enabled on macOS. Exit requests Cocoa's normal window state; both transitions refresh the OpenGL context and request redraw.
- Earlier restricted launches returned `kLSNoExecutableErr` and aborted inside AppKit. Launching outside the agent sandbox succeeds; these failures did not establish an application startup defect. The packaged app now launches on the real desktop, draws the original editor, enters/exits fullscreen, and displays a foreground cube render. No black output was observed in captured fullscreen frames. The original-reference parity gates remain open.


## 2026.10.14-alpha final validation

- Native Release build uses Apple Clang, arm64, `-O3 -DNDEBUG`; SDL and FreeType dependency builds also use Release configuration. The packaged executable is stripped and the bundle is ad-hoc signed. Distribution signing/notarization is outside this alpha.
- Python 2.5.6 is statically linked. Its standard library and 50 native extension modules are packaged under `Contents/Resources/python`; startup selects this bundled home. The regression check verifies the imported `os` module comes from the app, and exercises math, zlib, struct, expat and the legacy Blender API.
- Runtime data lives under `Contents/Resources/blender-data`; a relative `.blender` symlink preserves legacy resource lookup while allowing bundle signature verification. Dependency licenses are included under Resources.
- Fullscreen keeps the existing OpenGL view/context, updates and activates the context after resize, redraws both buffers, hides the macOS menu so Blender's exit button stays accessible, and restores the original window frame/title.
- `ctest --test-dir <build> --output-on-failure`: 4/4 passed (GHOST, dynamic loading, Python, TIFF). GHOST includes five repeated fullscreen transitions, pixel readback, frame/title restoration and disposal. The existing guarded allocator executable also passed.
- Actual packaged Python and TIFF checks passed. TIFF checks render to a temporary file then reload it; success requires an explicit marker because this legacy CLI can exit zero after a Python exception.
- `codesign --verify --deep --strict <app>` passed. Linkage inspection covered 57 Mach-O files, all arm64, with only system framework/library paths or resolving `@executable_path` references; no Homebrew or private dependency prefix is required at runtime.
- Static dependencies: Python, FreeType, main JPEG/PNG and gettext. Dynamic dependencies: SDL 1.2 compatibility/SDL2 and TIFF with JPEG, XZ and Zstd. SDL compatibility's upstream static development mode is Linux-only; the existing TIFF adapter uses runtime loading. This is not a fully static macOS executable.

Package after building:

```sh
tools/macos/package-release <build>/bin/blender.app <new-output-directory>
```

The ZIP preserves the `.app`, runtime symlink and signatures. `SHA256SUMS` uses a portable asset basename. Extract the ZIP before launching the app.

Final desktop acceptance: five fullscreen entry/exit cycles completed on the exact release app, with actual window bounds asserted after every transition and screenshots captured. Foreground render was triggered from the Render menu; its completed window was raised using the native window action. Foreground render and fullscreen/restored screenshots are under `docs/evidence/2026.10.14-alpha/`. Captured frames showed no black output; this bounded test does not establish stability across other displays or macOS versions.
