# 06: Keep uniform vertex colors stable under repeated blur

**What to build:** Artists can blur a uniformly colored mesh repeatedly without the division approximation darkening its vertex colors.

**Blocked by:** None (can start immediately)

**Status:** ready-for-agent

Approved by the maintainer on 2026-10-14. Blocked tickets remain non-executable until every linked prerequisite has a successful completion receipt. A no-go prerequisite does not unblock dependent integration.

- [ ] Reproduce darkening and freeze exact division-by-255 expectations for the affected byte blend operations.
- [ ] Adapt only the affected existing paint operations, retaining controls, mode choices, saturation and byte signedness.
- [ ] Test zero/full/intermediate factors, extrema and repeated uniform-color blur; untouched paint modes match the native oracle.
- [ ] Validate actual painting plus save/reopen and rendered colors; record affected output as an intentional correction rather than bitwise reference parity.
- [ ] Existing native checks pass.

## Parent

[Feature spec](../spec.md). This draft does not modify the existing native-build parent.

Upstream provenance: [46493c0af5e60f8841eaccb7d79b9537c9252406](https://github.com/blender/blender/commit/46493c0af5e60f8841eaccb7d79b9537c9252406).

## Comments

2026-10-14: Maintainer approved specification, testing seams, ticket granularity and blocking edges. Promoted to ready-for-agent; no implementation performed.
