# 03: Open projects with invalid fonts without crashing

**What to build:** Artists receive the existing controlled failure for missing or malformed packed fonts while valid text shapes and metrics remain unchanged.

**Type:** bug

**Blocked by:** None (can start immediately)

**Status:** ready-for-agent

Approved by the maintainer on 2026-10-14. Blocked tickets remain non-executable until every linked prerequisite has a successful completion receipt. A no-go prerequisite does not unblock dependent integration.

- [x] Reproduce a missing packed-font and FreeType face-creation failure before applying the narrow upstream return-path correction.
- [x] Invalid cases produce the legacy failure result without accessing an uninitialized face or leaking font resources.
- [x] Valid font outlines, metrics, text object geometry and representative rendered output match the frozen native oracle.
- [x] Actual packaged open/reload, foreground rendering and native checks pass; retain the original error/UI contract.

## Parent

[Feature spec](../spec.md). This draft does not modify the existing native-build parent.

Upstream provenance: [03de57c6d5a72b30014ed7753525af1df4feba93](https://github.com/blender/blender/commit/03de57c6d5a72b30014ed7753525af1df4feba93).

Verification boundary: [ADR 0006](../../../docs/adr/0006-keep-foreground-proof-separate-from-cli.md).

## Comments

2026-10-14: Maintainer approved specification, testing seams, ticket granularity and blocking edges. Promoted to ready-for-agent; no implementation performed.

2026-10-14: TDD reproduced a segfault in the native `freetype_failure_test` with malformed packed bytes before the correction. Ported upstream `03de57c6d5a72b30014ed7753525af1df4feba93`: return `FALSE` when temporary packed bytes are missing or `FT_New_Memory_Face` fails. Ensure the caller also releases the initialized FreeType library on this failure path. The regression exercises malformed bytes and a missing retained font buffer through `BLI_vfontchar_from_freetypefont`; both return the legacy false result without crashing. The bundled `bfont.ttf` control verifies glyph A has outline data and width `0.481112659`. Packaged CLI save/reopen verifies the text, font path, and 17 generated contours. Its 160x60 render matches the baseline generated from the pre-fix source exactly (SHA-256 `bf45f477b3cf1107d6e8f5af23eb8a54db8223328f6314d4e2abff1ad16ab499`); baseline: `../../tools/macos/tests/data/font-error-baseline.png`. Foreground app open with the saved scene is captured in [evidence](../evidence/font-text-foreground-open.png). Full arm64 build passes; full native CTest passes 9/9 outside the sandbox, and the malformed-font test passes under AddressSanitizer. The foreground render action remains unverified because the supported UI surface exposes no native input actions; status is ready-for-human for that required check, per ADR 0006.

2026-10-14: Rebuilt successfully and reran the full native CTest suite: 9/9 passed with host window access. The sandboxed run failed only `ghost_native` because of LaunchServices access. At this checkpoint, CUA reported no apps and lacked the documented native app lookup/launch methods, so the foreground action was still pending; it was completed in the following receipt.

2026-10-14: Launched the compiled `blender` executable with the saved `.blend` argument in the host desktop session. The Blender viewport showed the reopened text scene. Activated Blender's visible Render control with native mouse input; the `Blender:Render` window displayed the rendered “Blender 2.45” image and reported `Time:00:00.11`. Captured [foreground render evidence](../evidence/font-text-foreground-render.png). This completes the final acceptance check; CLI render remains byte-identical to the pre-fix baseline and full CTest passes 9/9 with host window access.
