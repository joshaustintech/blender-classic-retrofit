# 01: Compare safe owned-buffer channel conversion with C

**What to build:** Maintainers can run an exact safe Rust RGBA/ABGR operation against the optimized native C oracle and see realistic kernel and transfer costs.

**Blocked by:** [post-245-performance/01: Capture native workload profiles and output oracles](../../post-245-performance/issues/01-profile-and-oracles.md)

**Status:** ready-for-agent

Approved by the maintainer on 2026-10-14. Blocked tickets remain non-executable until every linked prerequisite has a successful completion receipt. A no-go prerequisite does not unblock dependent integration.

- [ ] Select representative profiled channel conversion inputs and freeze the exact existing channel permutation.
- [ ] Implement the scalar owned-buffer operation with stable stdlib safe Rust and checked dimensions/lengths; no C pointer slicing.
- [ ] Compare zero/odd/tail sizes, byte-only/float-only variants where supported, adversarial bit patterns and allocation/error behavior.
- [ ] Record optimized C versus safe Rust output and timings using identical data/layout/target settings; inspect emitted code rather than assert SIMD.
- [ ] Keep application defaults unchanged; deliver runnable evidence and a bounded integration/no-go decision.

## Parent

[Feature spec](../spec.md). This draft does not modify the existing native-build parent.

## Comments

2026-10-14: Maintainer approved specification, testing seams, ticket granularity and blocking edges. Promoted to ready-for-agent; no implementation performed.
