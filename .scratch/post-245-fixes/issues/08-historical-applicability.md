# 08: Resolve applicability of remaining historical fixes

**What to build:** Maintainers receive a source-backed apply/defer/reject decision for every remaining researched historical correctness item without importing later features.

**Blocked by:** None (can start immediately)

**Status:** ready-for-agent

Approved by the maintainer on 2026-10-14. Blocked tickets remain non-executable until every linked prerequisite has a successful completion receipt. A no-go prerequisite does not unblock dependent integration.

- [ ] Classify vector blur, script-execution controls, QMC leakage, area-light defects, keying-set crashes, empty-path helpers, scaler banding, Boolean orientation and non-mesh softbody initialization.
- [ ] For each item record immutable patch, current affected behavior, prerequisites, correction impact and a smallest reproducer or an explicit inapplicability reason.
- [ ] Confirm first-shipped release using a tag/archive or explicit evidence where feasible; retain uncertainty rather than infer shipment from trunk ancestry.
- [ ] Reject fixes for regressions absent in 2.45 and code tied to excluded/later features.
- [ ] Produce bounded follow-up proposals for applicable items; no runtime changes or automatic renderer/sandbox migration in this slice.

## Parent

[Feature spec](../spec.md). This draft does not modify the existing native-build parent.

## Comments

2026-10-14: Maintainer approved specification, testing seams, ticket granularity and blocking edges. Promoted to ready-for-agent; no implementation performed.
