# Qualify and backport upstream performance improvements

Status: ready-for-agent

Approved by the maintainer on 2026-10-14: specification, testing seams, ticket granularity and blocking edges. Implementation has not started.

## Problem Statement

Later Blender versions contain performance changes, but upstream anecdotes do not demonstrate a gain on the optimized Apple Silicon app. Users need faster existing operations without changed outputs or controls.

## Solution

Establish representative native measurements, optimize existing text traversal where justified, and qualify later render-conversion and memory changes with explicit applicability and fidelity decisions.

## User Stories

1. As an artist, I want existing editing workflows to be measured, so that optimizations address delays I actually experience.
2. As an artist, I want texture and image operations to be measured, so that large assets do not hide the important bottlenecks.
3. As an artist, I want foreground and background rendering to be measured separately, so that interactive latency and total throughput remain visible.
4. As a script author, I want reading long text blocks to avoid repeated traversal, so that large scripts can inspect text efficiently.
5. As a script author, I want text edits during iteration to remain correct, so that cached traversal does not introduce stale references.
6. As a script author, I want reset and end-of-file behavior to remain unchanged, so that existing iteration code keeps working.
7. As an artist, I want vector passes to preserve dependency evaluation, so that skipped conversions do not lose animation data.
8. As an artist, I want large scenes to use memory efficiently, so that rendering can fit within the machine budget.
9. As an artist, I want radiosity and render passes to survive memory-layout changes, so that optimization does not remove functionality.
10. As an artist, I want image scaling to preserve filters and rounding, so that faster scaling does not introduce banding.
11. As a maintainer, I want the current release build to be the performance baseline, so that debug-build comparisons do not inflate gains.
12. As a maintainer, I want timing distributions and workload details to be recorded, so that results can be reproduced.
13. As a maintainer, I want upstream claims to be separated from new local measurements, so that historical numbers are not misreported.
14. As a maintainer, I want rejected performance candidates to be recorded, so that future work does not repeat unhelpful experiments.
15. As a maintainer, I want large renderer changes to require a new bounded plan, so that the backport effort remains reviewable.

## Implementation Decisions

- Preserve feature parity, UX parity, the Python 2.5 API and Blender Internal. Retain original controls, defaults, progress, cancellation and file semantics.
- Keep Game Engine/player excluded. Import no later UI, shader, particle, modifier or renderer feature merely because it accompanies a useful patch.
- Use existing native application tests and packaged-app workflows as the primary acceptance boundary. Add a focused lower-level oracle only for behavior that cannot be observed reliably there.
- Use the research snapshot as provenance, then recheck the actual implementation branch before each change. Carry only the responsible patch and required correctness dependencies.
- Distinguish approved corrected behavior from unchanged behavior. Freeze a narrowly described failing-case expectation before fixing it; preserve unaffected outputs and never widen tolerances afterward.
- Measure representative editing, text reading, image operations, compositing and rendering with identical inputs, settings, seeds, release flags and thread counts. Record workload hashes and host/compiler context.
- Use repeated measurements and noise estimates; freeze a workload-specific practically worthwhile gain and permitted memory/latency regressions before evaluating a candidate. No arbitrary universal speedup percentage is preclaimed.
- For text traversal, adapt only the cached cursor and necessary reset/invalidation behavior. Do not import the new optional text-range API or editor plugins.
- Qualify adjacent-frame vector-pass conversion, per-face render storage and conservative object bounds separately; choose at most one narrow follow-up backed by the profile, not a broad combined renderer patch.
- Catalogue faster scaling with its later rounding fix, pow-free bicubic evaluation, instancing and KDOP/BVH improvements as conditional/deferred where routines or structures are absent or output contracts differ.
- Disabled X-mirror optimization and null-dereference fixes already absent from the baseline are not backport work. Game-engine and later texture-feature gains remain excluded.

## Testing Decisions

- Test observable results, errors and resource behavior rather than source structure or the presence of a particular optimization.
- Reuse the existing native CTest, embedded Python regression and image-format checks; extend those seams rather than build a parallel test framework.
- Exercise actual packaged startup, editing, save/reopen, foreground/background rendering and fullscreen for changes affecting those workflows. Compilation is not GUI or render-parity proof.
- Keep native-checkout regression evidence separate from original reference-release evidence. The original reference corpus is still an open gate; do not claim reference parity from the native oracle alone.
- Capture immutable native scalar/output oracles before measured transformations; validate decoded bytes exactly where possible and use only pre-agreed float scene/pass limits.
- For text traversal test empty lines, EOF, reset, insertion, write, deletion, reload and mutation outside the wrapper; no cached pointer may survive invalidating edits.

## Out of Scope

Unmeasured speed claims, rendering algorithm changes disguised as optimizations, new text API options, later feature import, blanket fast-math flags and automatic publication of a new binary release.

## Further Notes

The profiling slice produces usable evidence independently. A candidate may end with a no-change result if no practical gain survives correctness checks.

Research provenance: [post-2.45 research](../../docs/evidence/post-245-backports-and-safe-rust.md). Stable source contracts are described by module behavior rather than implementation file paths.

## Comments

2026-10-14: Maintainer approved specification, testing seams, ticket granularity and blocking edges. Promoted to ready-for-agent; no implementation performed.
