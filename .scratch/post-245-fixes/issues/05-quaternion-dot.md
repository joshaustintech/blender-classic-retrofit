# 05: Compute the legacy quaternion dot product correctly

**What to build:** Existing Python scripts receive the dot product of both operands instead of the first operand multiplied by itself.

**Blocked by:** None (can start immediately)

**Status:** ready-for-agent

Approved by the maintainer on 2026-10-14. Blocked tickets remain non-executable until every linked prerequisite has a successful completion receipt. A no-go prerequisite does not unblock dependent integration.

- [ ] A legacy Python regression using unequal quaternions fails before the fix and verifies the analytic corrected scalar result afterward.
- [ ] The operation remains the legacy scalar dot product, not Hamilton quaternion multiplication.
- [ ] Scalar/vector coercion, exception types, operand handling and unaffected equal-operand cases remain unchanged.
- [ ] Document only unequal-operand output as the approved correction; existing API checks pass in the packaged app.

## Parent

[Feature spec](../spec.md). This draft does not modify the existing native-build parent.

Upstream provenance: [434a994f30705a86bf429588138617d89fed7c1f](https://github.com/blender/blender/commit/434a994f30705a86bf429588138617d89fed7c1f).

## Comments

2026-10-14: Maintainer approved specification, testing seams, ticket granularity and blocking edges. Promoted to ready-for-agent; no implementation performed.
