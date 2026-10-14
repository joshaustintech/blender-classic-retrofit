# 02: Integrate faster safe batch channel conversion when proven

**What to build:** Artists receive the same channel-conversion results faster in the actual packaged app if the complete safe batch operation beats the native baseline.

**Blocked by:** [safe-rust-performance/01: Compare safe owned-buffer channel conversion with C](01-rgba-oracle.md); [safe-rust-safety/01: Prove a safe literal-path touch boundary](../../safe-rust-safety/issues/01-safe-boundary-probe.md)

**Status:** ready-for-agent

Approved by the maintainer on 2026-10-14. Blocked tickets remain non-executable until every linked prerequisite has a successful completion receipt. A no-go prerequisite does not unblock dependent integration.

- [ ] Proceed only after exact oracle checks and a viable safe bulk IO boundary; retain the existing C operation during expansion.
- [ ] Batch complete rows/images, preserving channels, alias/overlap contracts, dimensions, allocation failures, cancellation and Python-facing behavior.
- [ ] Measure complete startup, serialization, copies, IPC, allocation and steady-state costs across tiny/typical/large inputs against frozen budgets.
- [ ] Enable only measured profitable sizes/routes; do not add speculative thresholds or force helper use on slow workloads.
- [ ] Actual packaged editing/image/render workflows, signatures, resources and native tests pass; unsupported/failure cases are explicitly safe.
- [ ] If no total gain survives, leave C production behavior unchanged and record a successful no-go experiment.

## Parent

[Feature spec](../spec.md). This draft does not modify the existing native-build parent.

## Comments

2026-10-14: Maintainer approved specification, testing seams, ticket granularity and blocking edges. Promoted to ready-for-agent; no implementation performed.
