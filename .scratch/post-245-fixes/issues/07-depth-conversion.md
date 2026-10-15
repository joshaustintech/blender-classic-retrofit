# 07: Handle out-of-range depth conversion deterministically

**What to build:** Artists can render extreme depth scenes without platform-dependent out-of-range float-to-integer conversion artifacts.

**Blocked by:** None (can start immediately)

**Category:** bug

**Status:** ready-for-agent

Approved by the maintainer on 2026-10-14. Blocked tickets remain non-executable until every linked prerequisite has a successful completion receipt. A no-go prerequisite does not unblock dependent integration.

- [x] Reproduce an affected existing depth path and freeze corrected finite out-of-range expectations before editing.
- [x] Adapt the minimal upstream saturation behavior to the current depth routines; define NaN/infinity handling explicitly without claiming the patch alone specifies it.
- [x] Ordinary finite in-range depth retains C truncation; degenerate/extreme conversion cases satisfy the corrected fixture.
- [ ] Mask, clipping and relevant render-pass output comparisons confirm exact ordinary-scene preservation.
- [ ] Foreground render window is visually confirmed alongside passing background and repeated render/cancel workflows; no changed shading or default settings.
- [x] Existing native checks and available undefined-behavior diagnostics pass, with unsupported diagnostics stated.

## Parent

[Feature spec](../spec.md). This draft does not modify the existing native-build parent.

Upstream provenance: [2009ef810e5317cdbbf31caa36decb17947e225f](https://github.com/blender/blender/commit/2009ef810e5317cdbbf31caa36decb17947e225f).

## Comments

2026-10-14: Maintainer approved specification, testing seams, ticket granularity and blocking edges. Promoted to ready-for-agent; no implementation performed.

2026-10-14: Triage confirms this remains a bug in this checkout: `zbuffillAc4`, `zbuffillGL4`, `zbuffillGLinv4` and `zbuffillGL_onlyZ` cast interpolated depth directly to `int`; immutable upstream fix is 2009ef810e5317cdbbf31caa36decb17947e225f. Before fix, UBSan `float-cast-overflow` reproduced the out-of-range conversion. Added a tested shared saturating conversion: finite values clamp to `INT_MIN`/`INT_MAX`, in-range values retain C truncation, infinities clamp by sign, and NaN maps to the far-depth sentinel `INT_MAX` (the upstream patch does not specify NaN). A focused CTest covers both boundaries, both overflow directions, infinities, NaN and ordinary signed fractions; focused UBSan/float-cast-overflow passes. Full app build passes; native CTest passes 12/12 when run with WindowServer access. The 160x120 far-plane-crossing `previewcube` scene rendered twice in background with byte-identical PNGs. An 8192x8192 render received two SIGINTs, stopped without producing an image, then completed successfully on rerun. F12 was sent to the directly launched native app, but its render window was off-screen and could not be visually inspected; this visual acceptance item remains open. A separate file-browser process crashed during scene switching. No render or shading defaults changed. See ADR 0007 for exceptional-value policy.
