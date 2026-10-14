# Native macOS Blender 2.45 Build Plan

Status: needs-triage

Plan status: accepted by the user through `grill-with-docs` on 2026-10-13. Implementation has not started; the tracker status indicates that this plan still needs decomposition into executable tickets. Accepted decisions are recorded in `docs/adr/`; terminology is in `CONTEXT.md`.

## Outcome

A reproducible native Apple Silicon build of Blender 2.45 that launches on the developer's macOS environment, preserves all reference-release features and Blender-owned UX, and produces verified Blender Internal renders from both the GUI and command line. A linked executable, a graphics probe, or a render-only build is insufficient.

Initial target: the current M1 Pro/macOS 27.0.1 environment. Additional platform support, distribution signing, renderer enhancements, and performance backports follow this milestone.

## Accepted implementation choices

- Keep application UI, scene model, original renderer, render settings, and legacy Python API. Adapt platform and dependency boundaries rather than importing a newer Blender application.
- Modernize the existing CMake build. Pin compiler settings, SDK, dependency source hashes, patches, and runtime assets. Use isolated build/dependency directories; leave the developer's Python installations and preferences intact.
- Use Cocoa behind the existing GHOST interface with a legacy OpenGL context. Audit mature upstream Cocoa code before reuse; preserve the 2.45 event/coordinate contract. Native legacy OpenGL has passed an independent offscreen capability probe.
- Preserve the Python 2.5 behavior baseline. First prove a compatible native legacy runtime; do not substitute Python 3 or modern `bpy` APIs. Restore the reference scripts and resources only after manifest/difference review.
- Replace obsolete platform services only where needed, keeping original controls, formats, and semantics. QuickTime/SDL replacements must demonstrate equivalent behavior; missing codecs or CD functionality remain explicit parity failures.
- Keep Rust and opportunistic performance/stability work out of the baseline port. Later candidates require independent profiling or defect reproduction, upstream provenance, regression coverage, and unchanged outputs/UX. Rust remains optional, safe, and standard-library-only; a candidate requiring unsafe Rust does not meet that constraint.

## Delivery sequence and gates

### 0. Establish the reference and preservation matrix

Run the official Intel/Python 2.5 release on compatible macOS with its required runtime. Prefer a compatible Intel reference machine if available; otherwise conduct a bounded emulator/guest-graphics feasibility investigation. A Linux render oracle may supplement evidence but cannot establish Mac UX parity.

Inventory every shipped feature, script, resource, format, codec control, and external integration. Map each to an executable check. Record dependency-conditioned behavior explicitly. Reconcile the twelve missing Collada files and inspect the 29 script differences identified in `docs/evidence/reference-release.md`.

Capture clean-account startup, screenshots, interactions, scripts, scene files, and render outputs. Establish numeric tolerance limits from repeated reference runs and documented cross-architecture calculations; reference repeatability alone cannot justify an arbitrary tolerance. Lock the corpus and limits before application porting. If a numeric limit cannot yet be justified, that case stays unresolved.

**Gate:** runnable Mac reference, hashed corpus, complete preservation matrix, and approved comparison limits. Original reference execution remains unverified; this gate is open.

### 1. Prove dependencies and repair build configuration

Make CMake work with the installed toolchain, declare appropriate language standards, and correct genuine compiler/API errors. Build native dependencies in the isolated prefix, including Python, fonts/translations, image formats, and audio. Preserve game engine, player, physics, fluid simulation, and original enabled integrations. Confirm architecture with `file`/`lipo` and actual dependency resolution with `otool`.

Compile and run existing standalone module checks where relevant, including guarded allocation. Do not treat error suppression, silent feature disabling, or mixed Intel/ARM libraries as a build repair.

**Gate:** reproducible native module/dependency builds with recorded commands and results; no unresolved dependency substitutions.

### 2. Port the GHOST boundary and prove the original editor

Implement/audit Cocoa window creation, legacy context lifecycle, swaps, cursor/custom cursors, pointer and keyboard events, modifiers, text input, focus, clipboard, document opening, fullscreen, tablet behavior, and display changes. Follow the original screen/client coordinates and Y-axis conversions. Preserve logical UI size on Retina displays and existing application font/icon/theme data.

Launch the original editor, manipulate the default scene, select objects, open menus, split/change editors, undo/redo, and save/reopen. Check geometry/screenshots and input results against the reference at fixed client dimensions; test window chrome behavior separately.

**Gate:** real onscreen editor evidence and passing interaction checks. An empty Cocoa window or offscreen probe does not pass.

### 3. Make the original renderer and file model work natively

Audit 32-to-64-bit portability: pointer conversions, integer widths, SDNA structure sizes/alignment, allocator assumptions, serialization, image buffers, and threading. Keep render algorithms/settings unchanged; investigate undefined behavior rather than hiding it with flags.

Use reference-created fixtures covering supported geometry, materials/textures, lighting, shadows/ray tracing, transparency, animation, and relevant render options. Extend cases from the preservation matrix rather than assuming one cube covers rendering. Test reference-file loading and native save/reopen in both native and original builds for semantic round trips.

**Gate:** foreground F12 render and background CLI still/animation renders, correct dimensions/channels/passes, valid output files, image comparisons within locked limits, and successful cancellation/repeat rendering. Example CLI shape, with original argument order preserved:

```sh
blender -b fixtures/scene.blend -o output/frame_ -F PNG -t 1 -f 1
```

Repeat at relevant thread counts and with repeated runs. Capture timing and memory only as baseline evidence; no speedup claims yet.

### 4. Close feature and packaging parity

Finish original enabled image/movie/audio formats, codecs, bundled import/export scripts, player/game engine, simulation features, translations, and shipped plugin source builds. Recompile native plugins while preserving their exposed contracts; Intel plugin binaries cannot be used as native ARM substitutes. Retain original UI options and prove their outputs rather than removing unsupported entries.

Package the app, player, runtime/dependencies, scripts, resources, and licenses using verified manifests. Test startup and dependency discovery in a clean test account without relying on the developer's installed Python or saved preferences.

**Gate:** every preservation-matrix row passes against the actual packaged artifacts. Any missing reference feature leaves the milestone incomplete.

### 5. Run the complete acceptance gate

From a documented fresh build directory, reproduce the build and run all module, file round-trip, scripting, format, game-engine, simulation, UI, and rendering checks. Include resize/focus/fullscreen, repeated render/cancel, and repeated save/reopen stress cases. Use sanitizers where supported to investigate port-induced memory/undefined-behavior defects; record any diagnostic limitations.

**Gate:** native app/player architecture, real GUI launch and screenshots, foreground/background rendering receipts, fixed-tolerance comparison reports, complete resource/feature manifest, passing workflow checks, and reproducible commands. Clearly distinguish reference evidence, host capability probes, native module checks, and full application evidence.

## Stop conditions and unresolved facts

Missing reference execution, incompatible legacy Python, unavailable original codec behavior, or failed GHOST behavior is a blocker to claiming parity. Resolve the responsible seam or report the specific failure; do not weaken acceptance, remove a feature, or widen tolerances to close the milestone.

Exact original runtime versions, the viable reference runner, donor commits, dependency patches, fixture coverage, and numeric limits still require the measured work above. They are research outputs, not assumptions presented as settled facts.

## Next step

Publish individual local tickets in dependency order; mark a ticket `ready-for-agent` only when its inputs and checks are specified. Begin with reference-runner and preservation-matrix work. The user has confirmed shared understanding of the implementation choices, sequence, and acceptance gates.

## Comments

> *This was generated by AI during triage.*

2026-10-13: The maintainer approved [native-build ticket 01](issues/01-native-build-runs-locally.md) as `enhancement / ready-for-agent`. Its brief preserves these acceptance gates and adds concrete observed crash-free launch/workflow coverage. Application implementation and the reference-execution gate remain open.

2026-10-13 implementation override: The maintainer explicitly deferred the original-reference prerequisite for now and requested focus on a working native build. Application porting may proceed while reference execution, corpus capture, and locked parity comparisons remain open. This does not establish parity or permit silent feature removal.
