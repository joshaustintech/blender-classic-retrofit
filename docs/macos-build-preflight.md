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
