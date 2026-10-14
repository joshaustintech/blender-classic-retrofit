# 03: Qualify one bounded upstream render optimization

**What to build:** Maintainers receive an implementable small render optimization proposal, or a documented no-go, based on actual cost and unchanged output contracts.

**Blocked by:** [post-245-performance/01: Capture native workload profiles and output oracles](01-profile-and-oracles.md)

**Status:** ready-for-agent

Approved by the maintainer on 2026-10-14. Blocked tickets remain non-executable until every linked prerequisite has a successful completion receipt. A no-go prerequisite does not unblock dependent integration.

- [ ] Individually assess vector-pass adjacent-frame conversion, per-face render storage and conservative object bounds against the measured native workload.
- [ ] Catalogue scaling plus rounding corrections, pow-free bicubic work, instancing and KDOP/BVH as conditional/deferred rather than importing missing subsystems.
- [ ] For a selected candidate identify actual baseline applicability, minimal dependencies, pass/radiosity/animation impact and unchanged-output proof.
- [ ] Choose at most one bounded follow-up with frozen gain/memory/fidelity budgets, or justify no worthwhile candidate.
- [ ] Deliver a source-cited proposal and reproducible measurement; do not perform a broad render ownership refactor in this ticket.

## Parent

[Feature spec](../spec.md). This draft does not modify the existing native-build parent.

## Comments

2026-10-14: Maintainer approved specification, testing seams, ticket granularity and blocking edges. Promoted to ready-for-agent; no implementation performed.
