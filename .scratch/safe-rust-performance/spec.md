# Measure faster safe Rust kernels without changing UX

Status: ready-for-agent

Approved by the maintainer on 2026-10-14: specification, testing seams, ticket granularity and blocking edges. Implementation has not started.

## Problem Statement

A Rust rewrite could add startup, transfer and allocation costs or change output despite improving a tiny kernel benchmark. Users need demonstrably faster complete operations with the same visible behavior.

## Solution

Build exact owned-buffer oracles, compare optimized C and stable safe Rust, then integrate only a measured batch operation whose end-to-end gain survives safety and parity checks.

## User Stories

1. As an artist, I want channel conversion to retain all pixel values, so that images keep their exact colors and alpha.
2. As an artist, I want odd image sizes to remain correct, so that vector tails do not lose pixels.
3. As an artist, I want large image operations to improve total latency, so that faster arithmetic produces an actual app benefit.
4. As an artist, I want small operations to avoid helper overhead, so that optimization does not slow common work.
5. As an artist, I want HDR and alpha compositing to retain operation order, so that the rendered image remains faithful.
6. As an artist, I want allocation failures to preserve existing data, so that performance work does not damage images.
7. As an artist, I want cancellation and progress to remain responsive, so that batching does not freeze the interface.
8. As an artist, I want the packaged app to include its required helper, so that performance does not depend on developer tools.
9. As a script author, I want image and render APIs to remain unchanged, so that existing scripts need no migration.
10. As a maintainer, I want C and Rust comparisons to use identical release settings, so that language comparisons are fair.
11. As a maintainer, I want buffer copies and serialization to be timed, so that reported wins cover integration costs.
12. As a maintainer, I want scalar Rust output to be verified before SIMD, so that the source of a numerical regression remains clear.
13. As a maintainer, I want small and large workload distributions to be retained, so that one cherry-picked case does not set policy.
14. As a maintainer, I want an unprofitable rewrite to be rejected, so that the project keeps fewer moving parts.
15. As a maintainer, I want performance evidence to identify exact artifacts, so that benchmarks can be repeated against the delivered build.

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
- Use contiguous owned buffers and batch complete rows/images. Start with RGBA/ABGR permutation; introduce float alpha operations as a separate scalar experiment.
- Compare an unchanged optimized C oracle, safe Rust scalar loops and compiler-auto-vectorized loops. Record generated instructions rather than assuming language or syntax produces SIMD.
- Keep the original C path as an expand step while the safe candidate is tested. Change only the selected operation after proof; retain supported small-workload routing when it is measurably needed.
- Preserve byte signedness, channel ordering, clipping, alias/overlap handling, float width, expression grouping, alpha rules, division and saturation.
- Freeze end-to-end practical gain and regression budgets using the profiling ticket. Report medians, distribution/noise, total memory and startup/steady-state cost.
- No integration is required when ownership/transfer cannot satisfy the contract or the profile shows no meaningful cost. A measured rejection is a successful experiment, not a claimed speedup.

## Testing Decisions

- Test observable results, errors and resource behavior rather than source structure or the presence of a particular optimization.
- Reuse the existing native CTest, embedded Python regression and image-format checks; extend those seams rather than build a parallel test framework.
- Exercise actual packaged startup, editing, save/reopen, foreground/background rendering and fullscreen for changes affecting those workflows. Compilation is not GUI or render-parity proof.
- Keep native-checkout regression evidence separate from original reference-release evidence. The original reference corpus is still an open gate; do not claim reference parity from the native oracle alone.
- Compare byte outputs exactly across zero, odd and tail lengths, overlap cases, float-only/byte-only images and dimensions near checked bounds.
- Time complete operations including helper start, requests, responses, copies, allocation and packaging. Include tiny, typical and large workloads and interruption/failure cases.
- For scalar alpha candidates compare HDR, alpha boundaries, exceptional floats and representative scene passes before any live default changes.

## Out of Scope

A generic Rust rewrite, algorithm changes, microbenchmark-only claims, mandatory worker use on all sizes, dependencies, unsafe code, nightly portable SIMD and unrelated scene ownership refactors.

## Further Notes

The RGBA oracle and scalar alpha experiment each have useful outcomes without production integration. Production migration is gated by the measured boundary and exact output proof.

Research provenance: [post-2.45 research](../../docs/evidence/post-245-backports-and-safe-rust.md). Stable source contracts are described by module behavior rather than implementation file paths.

## Comments

2026-10-14: Maintainer approved specification, testing seams, ticket granularity and blocking edges. Promoted to ready-for-agent; no implementation performed.
