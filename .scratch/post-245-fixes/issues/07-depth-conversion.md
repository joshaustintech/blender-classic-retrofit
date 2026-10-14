# 07: Handle out-of-range depth conversion deterministically

**What to build:** Artists can render extreme depth scenes without platform-dependent out-of-range float-to-integer conversion artifacts.

**Blocked by:** None (can start immediately)

**Status:** ready-for-agent

Approved by the maintainer on 2026-10-14. Blocked tickets remain non-executable until every linked prerequisite has a successful completion receipt. A no-go prerequisite does not unblock dependent integration.

- [ ] Reproduce an affected existing depth path and freeze corrected finite out-of-range expectations before editing.
- [ ] Adapt the minimal upstream saturation behavior to the current depth routines; define NaN/infinity handling explicitly without claiming the patch alone specifies it.
- [ ] Ordinary finite depth, masks, clipping and relevant passes retain exact native results; degenerate/extreme cases satisfy the corrected fixture.
- [ ] Foreground/background render and repeated render/cancel workflows pass; no changed shading or default settings.
- [ ] Existing native checks and available undefined-behavior diagnostics pass, with unsupported diagnostics stated.

## Parent

[Feature spec](../spec.md). This draft does not modify the existing native-build parent.

Upstream provenance: [2009ef810e5317cdbbf31caa36decb17947e225f](https://github.com/blender/blender/commit/2009ef810e5317cdbbf31caa36decb17947e225f).

## Comments

2026-10-14: Maintainer approved specification, testing seams, ticket granularity and blocking edges. Promoted to ready-for-agent; no implementation performed.
