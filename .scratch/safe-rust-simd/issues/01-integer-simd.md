# 01: Demonstrate exact safe SIMD for channel conversion

**What to build:** Artists get identical pixel values while a qualified batch path uses demonstrated SIMD only where it improves total operation cost.

**Blocked by:** [safe-rust-performance/02: Integrate faster safe batch channel conversion when proven](../../safe-rust-performance/issues/02-rgba-integration.md)

**Status:** ready-for-agent

Approved by the maintainer on 2026-10-14. Blocked tickets remain non-executable until every linked prerequisite has a successful completion receipt. A no-go prerequisite does not unblock dependent integration.

- [ ] Use stable safe Rust loops and verify vector instructions in the actual target artifact; no nightly/crates/unsafe intrinsics.
- [ ] Compare every supported lane tail, odd width and boundary condition exactly with both scalar Rust and native C oracles.
- [ ] If byte blends are added only as a separately bounded follow-up, preserve division by 255 and saturation; do not expand this channel-permutation slice.
- [ ] Keep C/scalar paths available until total-cost benchmarks meet the frozen budgets and exact outputs pass.
- [ ] Run actual packaged image/render workflows; no SIMD or speed claim passes on source shape alone. A no-vectorization/no-gain receipt leaves the existing path unchanged.

## Parent

[Feature spec](../spec.md). This draft does not modify the existing native-build parent.

## Comments

2026-10-14: Maintainer approved specification, testing seams, ticket granularity and blocking edges. Promoted to ready-for-agent; no implementation performed.
