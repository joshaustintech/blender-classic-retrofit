# 02: Preserve names at the legacy length boundary

**What to build:** Artists can name, duplicate, save and reopen objects at the legacy name limit without reading unterminated data or changing normal suffix rules.

**Blocked by:** None (can start immediately)

**Status:** ready-for-agent

Approved by the maintainer on 2026-10-14. Blocked tickets remain non-executable until every linked prerequisite has a successful completion receipt. A no-go prerequisite does not unblock dependent integration.

- [ ] Reproduce the name-termination defect at exactly 21 bytes and freeze expected names before changing code.
- [ ] Cover 20, 21 and longer byte lengths, duplicate suffix generation, empty names and supported multibyte byte sequences through legacy Python naming operations.
- [ ] The responsible bound check is adapted without importing unrelated string or naming behavior.
- [ ] Save/reopen preserves object identity and approved names; sanitizer diagnostics are clean where supported and limitations are recorded.
- [ ] Existing native checks pass with failing-before/passing-after evidence.

## Parent

[Feature spec](../spec.md). This draft does not modify the existing native-build parent.

Upstream provenance: [ef0ea178b13b6642517f820a2baf3c316110d7ae](https://github.com/blender/blender/commit/ef0ea178b13b6642517f820a2baf3c316110d7ae).

## Comments

2026-10-14: Maintainer approved specification, testing seams, ticket granularity and blocking edges. Promoted to ready-for-agent; no implementation performed.
