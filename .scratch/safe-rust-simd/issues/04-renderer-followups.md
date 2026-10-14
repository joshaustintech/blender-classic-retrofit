# 04: Bound future filter and ray calculation experiments

**What to build:** Maintainers receive small evidence-backed follow-up specs for costly filter/ray work, or explicit deferrals, without replacing Blender Internal.

**Blocked by:** [post-245-performance/01: Capture native workload profiles and output oracles](../../post-245-performance/issues/01-profile-and-oracles.md)

**Status:** ready-for-agent

Approved by the maintainer on 2026-10-14. Blocked tickets remain non-executable until every linked prerequisite has a successful completion receipt. A no-go prerequisite does not unblock dependent integration.

- [ ] Map measured filter/scaling, ray intersection/traversal, shading, SSS and simulation cost to the existing algorithm and output contract.
- [ ] For each proposed follow-up specify owned inputs/outputs, safe integration feasibility, in-place/border/traversal/tie-breaking risks and expected transfer cost.
- [ ] Preserve acceleration structure, sample seeds/consumption, epsilon rules, per-pixel accumulation and deterministic merge order in any proposed first experiment.
- [ ] Attach required fixtures covering transparency/refraction/shadows, animation, cancellation and filter borders with frozen limits; do not invent tolerances from candidate output.
- [ ] Publish bounded follow-up proposals or deferrals only; no new BVH, sampler, renderer, parallel scheduling policy, unsafe bridge or dependency is implemented here.

## Parent

[Feature spec](../spec.md). This draft does not modify the existing native-build parent.

## Comments

2026-10-14: Maintainer approved specification, testing seams, ticket granularity and blocking edges. Promoted to ready-for-agent; no implementation performed.
