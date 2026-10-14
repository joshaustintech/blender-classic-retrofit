# 03: Evaluate faithful safe batched vertex transforms

**What to build:** Maintainers can demonstrate whether batching an existing transform in safe Rust preserves geometry while improving real calculation cost.

**Blocked by:** [post-245-performance/01: Capture native workload profiles and output oracles](../../post-245-performance/issues/01-profile-and-oracles.md); [safe-rust-safety/01: Prove a safe literal-path touch boundary](../../safe-rust-safety/issues/01-safe-boundary-probe.md)

**Status:** ready-for-agent

Approved by the maintainer on 2026-10-14. Blocked tickets remain non-executable until every linked prerequisite has a successful completion receipt. A no-go prerequisite does not unblock dependent integration.

- [ ] Select one existing matrix-vector transform from the profile and freeze layout, handedness, normal treatment and expression order.
- [ ] Build a safe owned-buffer scalar comparison, then inspect auto-vectorization for complete batched vertices under the same policy.
- [ ] Test bounds, geometry, normals, singular/degenerate cases, odd/tail batches, selection and animation expectations.
- [ ] Include serialization/copies and batching in total time and memory; no per-vertex process calls.
- [ ] Keep production defaults unchanged in this experiment; deliver a bounded integration proposal only if exact/locked-limit checks and practical gain pass.

## Parent

[Feature spec](../spec.md). This draft does not modify the existing native-build parent.

## Comments

2026-10-14: Maintainer approved specification, testing seams, ticket granularity and blocking edges. Promoted to ready-for-agent; no implementation performed.
