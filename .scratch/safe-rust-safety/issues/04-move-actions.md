# 04: Move assets safely across storage boundaries

**What to build:** Artists move assets using the original browser with literal names and correct same-filesystem and cross-filesystem behavior.

**Blocked by:** [safe-rust-safety/01: Prove a safe literal-path touch boundary](01-safe-boundary-probe.md); [safe-rust-safety/03: Copy assets safely with legacy file-browser behavior](03-copy-actions.md)

**Status:** ready-for-agent

Approved by the maintainer on 2026-10-14. Blocked tickets remain non-executable until every linked prerequisite has a successful completion receipt. A no-go prerequisite does not unblock dependent integration.

- [ ] Use the validated copy behavior for the cross-filesystem fallback, with the source removed only after the legacy success conditions are satisfied.
- [ ] Preserve rename semantics, overwrite decisions, target interpretation, permissions/symlinks and return/error behavior.
- [ ] Test actual cross-filesystem moves where available; explicitly distinguish any simulated error-path check from that host evidence.
- [ ] Failures/cancellation avoid premature source deletion; data contents and metadata remain consistent with the approved contract.
- [ ] Actual packaged browser moves and asset reload succeed; no shell interpolation, authored unsafe or new UI.

## Parent

[Feature spec](../spec.md). This draft does not modify the existing native-build parent.

## Comments

2026-10-14: Maintainer approved specification, testing seams, ticket granularity and blocking edges. Promoted to ready-for-agent; no implementation performed.
