# 04: Keep Python errors safe across project reloads

**What to build:** Script authors can raise exceptions, load another project and raise another exception without retained traceback references dereferencing stale Blender objects.

**Blocked by:** None (can start immediately)

**Status:** ready-for-agent

Approved by the maintainer on 2026-10-14. Blocked tickets remain non-executable until every linked prerequisite has a successful completion receipt. A no-go prerequisite does not unblock dependent integration.

- [ ] Reproduce error/reload/error with actual Python 2.5 wrapped Blender objects through the existing scripting seam.
- [ ] Clear the responsible retained traceback reference at the right lifecycle boundary; ignore game-engine changes.
- [ ] Exception text and useful traceback output remain available, and normal object/reference counts remain valid.
- [ ] Record the deliberate change to retained traceback inspection as a narrowly approved bug-correction expectation.
- [ ] Repeated reload/error cycles and ordinary valid scripts pass in the packaged app; native checks pass.

## Parent

[Feature spec](../spec.md). This draft does not modify the existing native-build parent.

Upstream provenance: [59a30d822fe5ecfafbb5676b2058b7ff8d3760ed](https://github.com/blender/blender/commit/59a30d822fe5ecfafbb5676b2058b7ff8d3760ed).

## Comments

2026-10-14: Maintainer approved specification, testing seams, ticket granularity and blocking edges. Promoted to ready-for-agent; no implementation performed.
