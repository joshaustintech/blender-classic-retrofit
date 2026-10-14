# 07: Qualify an exact owned legacy RNG state

**What to build:** Maintainers can reproduce the legacy random sequence with owned safe Rust state and decide whether safe integration preserves sampling and is practical.

**Blocked by:** None (can start immediately)

**Status:** ready-for-agent

Approved by the maintainer on 2026-10-14. Blocked tickets remain non-executable until every linked prerequisite has a successful completion receipt. A no-go prerequisite does not unblock dependent integration.

- [ ] Use stable stdlib safe Rust with explicit wrapping multiply/add, identical mask and exact seed mapping.
- [ ] Compare sequences against the existing C oracle for boundary seeds, repeated/reset state, shuffle behavior and relevant numeric conversions.
- [ ] Demonstrate identical state consumption for representative sampling operations; do not substitute a new generator or thread stream assignment.
- [ ] Document whether a policy-compatible whole-operation boundary exists without per-random-number IPC and what it would cost.
- [ ] Keep production RNG unchanged; return a bounded integration proposal or evidence-backed no-go rather than a fabricated speed claim.

## Parent

[Feature spec](../spec.md). This draft does not modify the existing native-build parent.

## Comments

2026-10-14: Maintainer approved specification, testing seams, ticket granularity and blocking edges. Promoted to ready-for-agent; no implementation performed.
