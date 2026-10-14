# 02: Read long text blocks without repeated traversal

**What to build:** Script authors can sequentially read long text blocks efficiently while edits, reset and EOF retain the original Python behavior.

**Blocked by:** [post-245-performance/01: Capture native workload profiles and output oracles](01-profile-and-oracles.md)

**Status:** ready-for-agent

Approved by the maintainer on 2026-10-14. Blocked tickets remain non-executable until every linked prerequisite has a successful completion receipt. A no-go prerequisite does not unblock dependent integration.

- [ ] Use the text profile/oracle to select the workload and reproduce repeated linked-list traversal.
- [ ] Adapt the cached traversal with all required mutation/reset invalidation; import neither optional range arguments nor editor plugin features.
- [ ] Test writes, insertions, deletions, reload, reset, empty lines, EOF and mutations outside the wrapper; no stale cursor survives invalidation.
- [ ] Keep legacy method signatures, results and errors unchanged.
- [ ] Compare complete operation timing on small/large texts against pre-agreed budgets; retain C unchanged if no worthwhile measured benefit or if safe invalidation cannot be bounded.
- [ ] Native and packaged Python checks pass; report the measured result without borrowing upstream wall-time numbers.

## Parent

[Feature spec](../spec.md). This draft does not modify the existing native-build parent.

Upstream provenance: [65d0ef3e746dd43e1f5480cd70434d34cecb6459](https://github.com/blender/blender/commit/65d0ef3e746dd43e1f5480cd70434d34cecb6459).

## Comments

2026-10-14: Maintainer approved specification, testing seams, ticket granularity and blocking edges. Promoted to ready-for-agent; no implementation performed.
