# Ticket 03: renderer backport qualification

Date: 2026-10-14

## Baseline evidence

The optimized Release build's reproducible render fixture is `release/datafiles/preview.blend`: 51 vertices, 17 faces, 150×150 output at 25%, OSA 5, one thread. It does not request vector passes. Eight background renders per pass measured three-pass medians of 0.391214, 0.410887 and 0.362459 seconds, with P95 values 0.515612, 0.500245 and 0.500017 seconds. Ten native F12 renders displayed 0.25 seconds at the UI's 10 ms resolution. These are small preview-scene timings, not a representative large-scene budget. The optimized Release Time Profiler sampled the process but could not resolve Blender functions; no renderer hotspot ranking is available. Full commands, hashes and limits are in [Ticket 09's baseline](ticket-09-performance-baseline.md).

## Candidate review

| Candidate | Blender 2.45 source | Applicability and cost evidence | Decision |
| --- | --- | --- | --- |
| Skip adjacent-frame vector conversion not needed by requested passes | [`pipeline.c`: `render_scene_needs_vector`](../../source/blender/render/intern/source/pipeline.c#L577) gates vector conversion on an enabled layer with `SCE_PASS_VECTOR`; [`convertblender.c`: `RE_Database_FromScene_Vectors`](../../source/blender/render/intern/source/convertblender.c#L3817) builds the previous frame, next frame, then center-frame database. | The control-flow exists in this branch, but the measured scene requests no vector pass and never enters the path. No animated/vector-pass workload, unchanged-vector oracle or timing exists, so a gain is unproven. A valid experiment must preserve frame evaluation, duplicates, all enabled layers and vector outputs. | No-go pending a representative animated vector-pass fixture/profile. |
| Move radiosity-only per-face data out of `VlakRen` | [`render_types.h`](../../source/blender/render/intern/include/render_types.h#L267) keeps `RadFace *radface` in each face record; [`renderdatabase.c`](../../source/blender/render/intern/source/renderdatabase.c#L442) allocates face records in blocks; [`zbuf.c`](../../source/blender/render/intern/source/zbuf.c#L1921) consumes the pointer in radiosity. | This is a memory-layout change, not a measured speed hotspot. The fixture has only 17 faces; there is no large mesh/radiosity peak-memory baseline. Moving storage would touch allocation, copies, free paths and radiosity access. | No-go without a representative large-scene memory profile and radiosity/pass output oracles. |
| Add conservative object bounds before z-buffer work | Existing render code has face/tile and shadow BSP bounds, but no object-level frustum-bound cull in the Blender Internal conversion/z-buffer path. The relevant conversion is in [`convertblender.c`](../../source/blender/render/intern/source/convertblender.c); clipping and object rasterization are in [`zbuf.c`](../../source/blender/render/intern/source/zbuf.c). | This adds a culling algorithm rather than adapting an existing hot routine. No profile shows object conversion or z-buffering as dominant. Displaced and animated geometry, shadow/radiosity passes, tiles and border rendering all constrain conservative rejection. | No-go: speculative algorithmic scope without cost evidence. |

Later scaling/rounding, pow-free bicubic, instancing and KDOP/BVH changes remain conditional or deferred as catalogued in the [post-2.45 research](post-245-backports-and-safe-rust.md); several rely on absent later routines or broad ownership changes. Their upstream speed figures are not treated as measurements of this application.

## Outcome

No bounded render optimization is selected. The native render profile demonstrates correctness and completion time for a tiny ordinary scene, but does not exercise or attribute cost to the named candidates. A proposal would therefore invent a target and budget. Reopen only with a representative animated vector-pass scene or large radiosity mesh, reproducible output/pass oracles, and a profile that identifies the responsible cost.
