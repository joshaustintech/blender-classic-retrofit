# Backport verified post-2.45 correctness fixes

Status: ready-for-agent

Approved by the maintainer on 2026-10-14: specification, testing seams, ticket granularity and blocking edges. Implementation has not started.

## Problem Statement

The native app retains specific correctness and lifetime defects fixed upstream after 2.45 through 2.49b. Users need reliable files, fonts, scripting and painting without importing later Blender behavior wholesale.

## Solution

Apply small, reproduced fixes to existing workflows. Record intended corrections precisely, preserve all unrelated behavior and classify historical fixes that do not apply to this tree.

## User Stories

1. As an artist, I want repeated packed-file checks to release file descriptors, so that long sessions do not exhaust file resources.
2. As an artist, I want equal and different packed-file comparisons to retain their original statuses, so that packing decisions remain predictable.
3. As an artist, I want 21-character names to be safely terminated, so that renaming and saving do not corrupt data.
4. As an artist, I want duplicate and long names to retain familiar suffix rules, so that object identities remain understandable.
5. As an artist, I want missing or malformed fonts to fail cleanly, so that opening a project does not crash.
6. As an artist, I want valid font outlines and metrics to remain unchanged, so that text scenes preserve their appearance.
7. As a script author, I want exceptions followed by file reloads to avoid stale Blender references, so that repeated scripting remains stable.
8. As a script author, I want traceback messages to retain useful information, so that debugging still works after lifetime cleanup.
9. As a script author, I want quaternion multiplication to compute the legacy dot product using both operands, so that numerical scripts receive correct results.
10. As a script author, I want scalar and vector quaternion operations to retain their API contracts, so that existing scripts keep running.
11. As an artist, I want repeated blur painting of a uniform color to remain uniform, so that painting does not darken because of rounding errors.
12. As an artist, I want extreme depth values to behave deterministically, so that depth artifacts do not depend on undefined conversions.
13. As a maintainer, I want each fix to identify its immutable upstream patch and current applicability, so that later regressions are not mistakenly transplanted.
14. As a maintainer, I want corrected bug outputs to be distinguished from compatibility outputs, so that render-parity claims remain honest.
15. As a maintainer, I want historical feature-specific fixes to be marked deferred or inapplicable, so that the app keeps its 2.45 scope.

## Implementation Decisions

- Preserve feature parity, UX parity, the Python 2.5 API and Blender Internal. Retain original controls, defaults, progress, cancellation and file semantics.
- Keep Game Engine/player excluded. Import no later UI, shader, particle, modifier or renderer feature merely because it accompanies a useful patch.
- Use existing native application tests and packaged-app workflows as the primary acceptance boundary. Add a focused lower-level oracle only for behavior that cannot be observed reliably there.
- Use the research snapshot as provenance, then recheck the actual implementation branch before each change. Carry only the responsible patch and required correctness dependencies.
- Distinguish approved corrected behavior from unchanged behavior. Freeze a narrowly described failing-case expectation before fixing it; preserve unaffected outputs and never widen tolerances afterward.
- Initial direct fixes cover packed-file descriptor cleanup, name termination at the legacy limit, FreeType failure handling, Python traceback reference cleanup, quaternion dot-product operands, vertex-paint division rounding and out-of-range depth conversion.
- Legacy quaternion multiplication remains a scalar dot product; do not replace it with Hamilton multiplication.
- Painting and depth corrections are limited compatibility exceptions for reproduced defects, not permission to alter normal shading, filtering or default rendering.
- Classify vector blur, script-execution controls, QMC shadow leakage, area-light translucency/borders, keying-set crashes, empty-path helpers, scaler banding, negative-scale Booleans and softbody initialization against current code before proposing further implementation.
- Check actual source/tag or release archive evidence where first-shipped releases are uncertain; release markers alone do not certify tarball contents.

## Testing Decisions

- Test observable results, errors and resource behavior rather than source structure or the presence of a particular optimization.
- Reuse the existing native CTest, embedded Python regression and image-format checks; extend those seams rather than build a parallel test framework.
- Exercise actual packaged startup, editing, save/reopen, foreground/background rendering and fullscreen for changes affecting those workflows. Compilation is not GUI or render-parity proof.
- Keep native-checkout regression evidence separate from original reference-release evidence. The original reference corpus is still an open gate; do not claim reference parity from the native oracle alone.
- Use public legacy Python operations for names, quaternion results and reload/error cycles; verify resource counts and packed-file comparison statuses at the highest practical existing seam.
- Use corrupt/missing font fixtures plus valid outline/font-metric controls. Compare painting and depth fixtures against their frozen corrected expectations and unaffected native outputs.

## Out of Scope

Later Blender features, wholesale cherry-picks, a new script sandbox, a new renderer, broad allocator rewrites and speculative fixes absent from the current tree.

## Further Notes

Seven narrow correction slices can proceed independently after plan approval. The historical applicability audit is independently verifiable and produces follow-up proposals rather than automatic implementation.

Research provenance: [post-2.45 research](../../docs/evidence/post-245-backports-and-safe-rust.md). Stable source contracts are described by module behavior rather than implementation file paths.

## Comments

2026-10-14: Maintainer approved specification, testing seams, ticket granularity and blocking edges. Promoted to ready-for-agent; no implementation performed.
