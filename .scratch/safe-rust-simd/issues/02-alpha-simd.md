# 02: Evaluate safe SIMD alpha lanes with fixed arithmetic

**What to build:** Artists retain faithful alpha/HDR output while an independent-pixel SIMD alpha candidate is accepted only if it matches the scalar oracle and improves full operation cost.

**Blocked by:** [safe-rust-performance/03: Qualify an exact safe scalar alpha operation](../../safe-rust-performance/issues/03-alpha-oracle.md); [safe-rust-safety/01: Prove a safe literal-path touch boundary](../../safe-rust-safety/issues/01-safe-boundary-probe.md)

**Status:** ready-for-agent

Approved by the maintainer on 2026-10-14. Blocked tickets remain non-executable until every linked prerequisite has a successful completion receipt. A no-go prerequisite does not unblock dependent integration.

- [ ] Proceed only with a successful scalar alpha oracle and viable batch IO contract; vectorize that single operation.
- [ ] Preserve per-lane arithmetic ordering, branches and float width; no changed reduction order, fast math, approximate reciprocal/rsqrt or fused replacement.
- [ ] Compare tails, exceptional values, signed zero, subnormals, HDR/alpha and relevant rendered passes under pre-agreed limits.
- [ ] Inspect actual vector instructions and benchmark copies/IPC/allocation/cancellation along with arithmetic.
- [ ] Integrate only if fidelity and total-cost gates pass; otherwise preserve production scalar/C paths and record the reason.

## Parent

[Feature spec](../spec.md). This draft does not modify the existing native-build parent.

## Comments

2026-10-14: Maintainer approved specification, testing seams, ticket granularity and blocking edges. Promoted to ready-for-agent; no implementation performed.
