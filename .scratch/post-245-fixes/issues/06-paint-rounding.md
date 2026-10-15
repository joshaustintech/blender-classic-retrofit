# 06: Keep uniform vertex colors stable under repeated blur

**What to build:** Artists can blur a uniformly colored mesh repeatedly without the division approximation darkening its vertex colors.

**Blocked by:** None (can start immediately)

**Status:** ready-for-agent

Approved by the maintainer on 2026-10-14. Blocked tickets remain non-executable until every linked prerequisite has a successful completion receipt. A no-go prerequisite does not unblock dependent integration.

- [x] Reproduce darkening and freeze exact division-by-255 expectations for the affected byte blend operations.
- [x] Adapt only the affected existing paint operations, retaining controls, mode choices, saturation and byte signedness.
- [x] Test zero/full/intermediate factors, extrema and repeated uniform-color blur; untouched paint modes match the native oracle.
- [x] Validate actual painting plus save/reopen and rendered colors; record affected output as an intentional correction rather than bitwise reference parity.
- [x] Existing native checks pass.

## Parent

[Feature spec](../spec.md). This draft does not modify the existing native-build parent.

Upstream provenance: [46493c0af5e60f8841eaccb7d79b9537c9252406](https://github.com/blender/blender/commit/46493c0af5e60f8841eaccb7d79b9537c9252406).

## Comments

2026-10-14: Maintainer approved specification, testing seams, ticket granularity and blocking edges. Promoted to ready-for-agent; no implementation performed.

2026-10-14: Native arithmetic regression initially failed: 100 uniform Filter blends darkened byte channels 127/100/80 to 126/99/79, and multiply differed by one. Moved the existing packed-color helpers into `vpaint_color.c` for direct testing; changed only their `>>8`/`/256` arithmetic to `/255`, retaining `char` channel access, saturation, factor branches, paint dispatch, and mode controls. Regression covers all six modes, 0/255/intermediate factors, byte extrema, repeated uniform blending, and unchanged lighten/darken selection. Packaged Blender 2.45 UI was opened directly with a uniform quad, Vertex Paint > Filter selected, and 80 native brush strokes applied. Saved `/private/tmp/ticket06-simple.blend`; reopening with bundled Python 2.5 verified all four vertex colors remained `(64, 80, 100, 255)`, then Blender rendered the saved scene. See [foreground paint evidence](../evidence/vpaint-uniform-blur-foreground.png) and [rendered color evidence](../evidence/vpaint-uniform-blur-render.png). The corrected arithmetic is a deliberate output correction and is not a bitwise reference-parity claim. Full native CTest passes 11/11.
