# 08: Resolve applicability of remaining historical fixes

**What to build:** Maintainers receive a source-backed apply/defer/reject decision for every remaining researched historical correctness item without importing later features.

**Blocked by:** None (can start immediately)

**Status:** ready-for-agent
**Category:** enhancement

Approved by the maintainer on 2026-10-14. Blocked tickets remain non-executable until every linked prerequisite has a successful completion receipt. A no-go prerequisite does not unblock dependent integration.

- [ ] Classify vector blur, script-execution controls, QMC leakage, area-light defects, keying-set crashes, empty-path helpers, scaler banding, Boolean orientation and non-mesh softbody initialization.
- [ ] For each item record immutable patch, current affected behavior, prerequisites, correction impact and a smallest reproducer or an explicit inapplicability reason.
- [ ] Confirm first-shipped release using a tag/archive or explicit evidence where feasible; retain uncertainty rather than infer shipment from trunk ancestry.
- [ ] Reject fixes for regressions absent in 2.45 and code tied to excluded/later features.
- [ ] Produce bounded follow-up proposals for applicable items; no runtime changes or automatic renderer/sandbox migration in this slice.

**Receipt (2026-10-14):** Completed source and immutable-upstream-patch audit in [historical applicability report](../../../docs/evidence/ticket-08-historical-applicability.md). Applicable follow-up candidates: vector-blur depth interpolation, Python-driver execution control, QMC ray-shadow leakage, negative-scale Boolean orientation, and non-mesh softbody point initialization. Rejected as already present or tied to absent/later code: the area-light early-return patch, keying-set helper, empty-path slash helper, and later quick-scaler rounding patch. Each applicable item includes its smallest reproduction, prerequisites, expected output/behavior impact, and release-evidence confidence; exact stable-release inclusion remains uncertain where no archive/tag proves it. Runtime source unchanged. Verification: `cmake --build /private/tmp/retrofit-ticket01-build --parallel 8` passed; `ctest --test-dir /private/tmp/retrofit-ticket01-build --output-on-failure` passed 12/12 with macOS WindowServer access; `git diff --check` passed. Standards and Spec reviews passed after the reviewers' findings were addressed. No runtime changes, so TDD and performance benchmarking were not applicable.

- [x] Classify vector blur, script-execution controls, QMC leakage, area-light defects, keying-set crashes, empty-path helpers, scaler banding, Boolean orientation and non-mesh softbody initialization.
- [x] For each item record immutable patch, current affected behavior, prerequisites, correction impact and a smallest reproducer or an explicit inapplicability reason.
- [x] Confirm first-shipped release using a tag/archive or explicit evidence where feasible; retain uncertainty rather than infer shipment from trunk ancestry.
- [x] Reject fixes for regressions absent in 2.45 and code tied to excluded/later features.
- [x] Produce bounded follow-up proposals for applicable items; no runtime changes or automatic renderer/sandbox migration in this slice.

## Parent

[Feature spec](../spec.md). This draft does not modify the existing native-build parent.

## Comments

2026-10-14: Maintainer approved specification, testing seams, ticket granularity and blocking edges. Promoted to ready-for-agent; no implementation performed.

2026-10-14: Audit receipt above records five applicable follow-up proposals and four rejected/inapplicable patch families. No runtime code changed.
