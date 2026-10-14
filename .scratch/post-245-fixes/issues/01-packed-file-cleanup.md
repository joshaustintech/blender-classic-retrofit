# 01: Keep repeated packed-file checks resource-stable

**What to build:** Artists can repeatedly compare packed assets against disk files without leaking descriptors or changing comparison results.

**Blocked by:** None (can start immediately)

**Status:** ready-for-agent

Approved by the maintainer on 2026-10-14. Blocked tickets remain non-executable until every linked prerequisite has a successful completion receipt. A no-go prerequisite does not unblock dependent integration.

- [ ] Reproduce descriptor growth through the existing packed-asset comparison behavior before applying the minimal upstream cleanup.
- [ ] Equal, different, missing and interrupted/truncated-read cases keep their documented legacy result codes; every successfully opened descriptor is released.
- [ ] Repeated comparisons have a stable descriptor count and valid packed assets survive save/reopen.
- [ ] Existing native regression checks pass; record upstream provenance and a reproducible failing-before/passing-after receipt.

## Parent

[Feature spec](../spec.md). This draft does not modify the existing native-build parent.

Upstream provenance: [e60138117caaafc0f9b422e5e3f1654127e4287a](https://github.com/blender/blender/commit/e60138117caaafc0f9b422e5e3f1654127e4287a).

## Comments

2026-10-14: Maintainer approved specification, testing seams, ticket granularity and blocking edges. Promoted to ready-for-agent; no implementation performed.
