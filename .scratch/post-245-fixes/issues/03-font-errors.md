# 03: Open projects with invalid fonts without crashing

**What to build:** Artists receive the existing controlled failure for missing or malformed packed fonts while valid text shapes and metrics remain unchanged.

**Blocked by:** None (can start immediately)

**Status:** ready-for-agent

Approved by the maintainer on 2026-10-14. Blocked tickets remain non-executable until every linked prerequisite has a successful completion receipt. A no-go prerequisite does not unblock dependent integration.

- [ ] Reproduce a missing packed-font and FreeType face-creation failure before applying the narrow upstream return-path correction.
- [ ] Invalid cases produce the legacy failure result without accessing an uninitialized face or leaking font resources.
- [ ] Valid font outlines, metrics, text object geometry and representative rendered output match the frozen native oracle.
- [ ] Actual packaged open/reload, foreground rendering and native checks pass; retain the original error/UI contract.

## Parent

[Feature spec](../spec.md). This draft does not modify the existing native-build parent.

Upstream provenance: [03de57c6d5a72b30014ed7753525af1df4feba93](https://github.com/blender/blender/commit/03de57c6d5a72b30014ed7753525af1df4feba93).

## Comments

2026-10-14: Maintainer approved specification, testing seams, ticket granularity and blocking edges. Promoted to ready-for-agent; no implementation performed.
