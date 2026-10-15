# 02: Preserve names at the legacy length boundary

**What to build:** Artists can name, duplicate, save and reopen objects at the legacy name limit without reading unterminated data or changing normal suffix rules.

**Type:** bug

**Blocked by:** None (can start immediately)

**Status:** ready-for-human

Approved by the maintainer on 2026-10-14. Blocked tickets remain non-executable until every linked prerequisite has a successful completion receipt. A no-go prerequisite does not unblock dependent integration.

- [x] Reproduce the name-termination defect at exactly 21 bytes and freeze expected names before changing code.
- [x] Cover 20, 21 and longer byte lengths, duplicate suffix generation, empty names and supported multibyte byte sequences through legacy Python naming operations.
- [x] The responsible bound check is adapted without importing unrelated string or naming behavior.
- [x] Save/reopen preserves object identity and approved names; sanitizer diagnostics are clean where supported and limitations are recorded.
- [x] Existing native checks pass with failing-before/passing-after evidence.

## Parent

[Feature spec](../spec.md). This draft does not modify the existing native-build parent.

Upstream provenance: [ef0ea178b13b6642517f820a2baf3c316110d7ae](https://github.com/blender/blender/commit/ef0ea178b13b6642517f820a2baf3c316110d7ae).

## Comments

2026-10-14: Maintainer approved specification, testing seams, ticket granularity and blocking edges. Promoted to ready-for-agent; no implementation performed.

2026-10-14: Applied the provenance-backed `result >= 21` terminator fix in `library.c`. Added legacy-Python object naming coverage for 20, 21 and 22 ASCII bytes, ordinary and long-name duplicates, an empty name and a 20-byte multibyte name. The test stores per-object ID properties, saves the project, reloads it in a second Blender process and verifies each named object's identity property. Red proof: with the pre-fix `result > 21` condition, the ASan build's `ctest --test-dir /private/tmp/retrofit-ticket02-asan --output-on-failure -R '^name_boundary$'` fails with a `stack-buffer-overflow` in `check_for_dupid`, reading past local `name[22]` in `new_id`. Green proof: after changing the condition to `>= 21`, that ASan CTest passes. Standard arm64 build and focused CTest pass. The full suite passes 7/7 when run outside the filesystem sandbox; `ghost_native` cannot initialize a Cocoa window inside the sandbox, so sandboxed full-suite runs are expected to fail that unrelated UI test. ASan bundling required temporarily adding the compiler runtime directory to the local CMake bundler search list; the workaround was reverted and is not part of the ticket diff.
