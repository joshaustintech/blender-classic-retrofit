# Qualify fidelity-preserving SIMD and expensive calculation seams

Status: ready-for-agent

Approved by the maintainer on 2026-10-14: specification, testing seams, ticket granularity and blocking edges. Implementation has not started.

## Problem Statement

Rendering and other numeric workloads may benefit from vectorization, but changed rounding, traversal or sampling can alter images. Users need faster calculations without compromised visual fidelity.

## Solution

Vectorize independent exact pixel work first using stable safe Rust loops, then separately evaluate alpha lanes and batched transforms. Bound later renderer/filter work behind profiles and explicit fidelity decisions.

## User Stories

1. As an artist, I want SIMD pixel operations to preserve exact decoded bytes, so that optimization cannot change texture appearance.
2. As an artist, I want SIMD tails to work at every supported size, so that odd dimensions remain supported.
3. As an artist, I want float alpha operations to retain source arithmetic, so that transparency and HDR output remain faithful.
4. As an artist, I want vertex transforms to preserve matrix conventions, so that geometry and normals do not move incorrectly.
5. As an artist, I want degenerate and singular cases to remain predictable, so that rare scenes do not break faster calculations.
6. As an artist, I want sample seeds and consumption order to remain stable, so that renders are reproducible.
7. As an artist, I want per-pixel accumulation order to remain stable, so that parallel work does not alter shading.
8. As an artist, I want shadow, reflection and refraction behavior to remain unchanged, so that ray acceleration preserves scene appearance.
9. As an artist, I want cancellation and progress to keep their original behavior, so that expensive batches remain interactive.
10. As a maintainer, I want the generated Apple Silicon instructions to be inspected, so that SIMD is demonstrated rather than assumed.
11. As a maintainer, I want stable Rust without unsafe code to remain the default, so that vectorization preserves project policy.
12. As a maintainer, I want nightly or SIMD-crate proposals to be kept separate, so that research does not silently authorize new dependencies.
13. As a maintainer, I want fast math and fused substitutions to be excluded, so that throughput does not redefine numeric semantics.
14. As a maintainer, I want native and reference fidelity receipts to be distinguished, so that local equality is not misreported as reference parity.
15. As a maintainer, I want ray and filter ownership changes to have bounded follow-up plans, so that renderer work remains reviewable.
16. As a maintainer, I want failed SIMD experiments to retain the scalar implementation, so that a hardware optimization is never forced into production.

## Implementation Decisions

- Preserve feature parity, UX parity, the Python 2.5 API and Blender Internal. Retain original controls, defaults, progress, cancellation and file semantics.
- Keep Game Engine/player excluded. Import no later UI, shader, particle, modifier or renderer feature merely because it accompanies a useful patch.
- Use existing native application tests and packaged-app workflows as the primary acceptance boundary. Add a focused lower-level oracle only for behavior that cannot be observed reliably there.
- Use the research snapshot as provenance, then recheck the actual implementation branch before each change. Carry only the responsible patch and required correctness dependencies.
- Distinguish approved corrected behavior from unchanged behavior. Freeze a narrowly described failing-case expectation before fixing it; preserve unaffected outputs and never widen tolerances afterward.
- Use stable, standard-library-only Rust with a mechanically enforced prohibition on authored unsafe Rust. No unsafe attributes, pointer-to-slice conversion or hidden authored unsafe bridge.
- An owned-buffer helper through ordinary IO is a candidate integration boundary, not an already approved performance solution. Demonstrate one complete operation and its errors/cancellation before expanding it.
- No per-pixel, per-vertex or per-random-number process calls. Include process startup, serialization, buffer copies, IPC, allocation and packaging in total cost.
- If the safe integration boundary cannot preserve behavior or is impractical, retain the C path and finish with a documented no-go result. No forced migration to meet a language target.
- Standard-library/compiler/OS implementation internals remain trusted dependencies; the authored-safe constraint does not mean every dependency contains zero unsafe code.
- Begin with stable safe slice/array loops and inspect compiler-emitted vector instructions on the actual target. Explicit portable SIMD remains a separate unapproved nightly option.
- Use independent pixels or vertices as lanes; preserve scalar expression ordering inside each lane. No changed reduction ordering, approximate reciprocal/rsqrt, fast-math or casual fused multiply-add.
- Integer blend division by 255 must not be substituted with a shift dividing by 256. Match clipping, saturation, widths and the native unsigned-byte contract.
- Keep scalar oracles and an unchanged C comparison until each vectorized operation passes its own exact or locked-limit checks and total-cost measurement.
- For transforms, batch multiple vertices per matrix while preserving layout, handedness, normal treatment and singular behavior. Do not redesign the mesh model.
- For later rays and filters, map profiling cost and risks without replacing traversal, sample generation, acceleration structure, border rules or in-place update order in this scope.
- Any future parallel tile work must own disjoint outputs and retain deterministic seeds, per-pixel accumulation and merge order.

## Testing Decisions

- Test observable results, errors and resource behavior rather than source structure or the presence of a particular optimization.
- Reuse the existing native CTest, embedded Python regression and image-format checks; extend those seams rather than build a parallel test framework.
- Exercise actual packaged startup, editing, save/reopen, foreground/background rendering and fullscreen for changes affecting those workflows. Compilation is not GUI or render-parity proof.
- Keep native-checkout regression evidence separate from original reference-release evidence. The original reference corpus is still an open gate; do not claim reference parity from the native oracle alone.
- Inspect emitted vector instructions and test all scalar tails, odd widths, zero sizes and alias/overlap cases against exact byte oracles.
- Use locked scalar float expectations for signed zero, exceptional values, subnormals, HDR/alpha and filter borders; verify output scene passes and geometry alongside total timings.
- For transform experiments compare geometry, normals, bounds and selection/animation behavior; include singular transforms and representative batched sizes.
- No ray/filter performance claim may pass without a matching shadow/transparency/refraction/animation corpus, stable RNG behavior and recorded numerical limits.

## Out of Scope

Nightly adoption, third-party SIMD crates, unsafe NEON intrinsics, fast math, renderer replacement, new BVH/sampler, complete shading/SSS/simulation rewrites and automatic wider ownership migration.

## Further Notes

Later renderer opportunities are tracked as bounded qualification work. The native baseline may be used for local regression experiments; original reference-release parity remains separately open.

Research provenance: [post-2.45 research](../../docs/evidence/post-245-backports-and-safe-rust.md). Stable source contracts are described by module behavior rather than implementation file paths.

## Comments

2026-10-14: Maintainer approved specification, testing seams, ticket granularity and blocking edges. Promoted to ready-for-agent; no implementation performed.
