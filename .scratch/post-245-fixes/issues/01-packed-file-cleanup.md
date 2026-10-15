# 01: Keep repeated packed-file checks resource-stable

**What to build:** Artists can repeatedly compare packed assets against disk files without leaking descriptors or changing comparison results.

**Blocked by:** None (can start immediately)

**Status:** ready-for-agent

Approved by the maintainer on 2026-10-14. Blocked tickets remain non-executable until every linked prerequisite has a successful completion receipt. A no-go prerequisite does not unblock dependent integration.

- [ ] Reproduce descriptor growth through the existing packed-asset comparison behavior before applying the minimal upstream cleanup.
- [ ] Equal, different, missing and interrupted/truncated-read cases keep their documented legacy result codes; every successfully opened descriptor is released.
- [ ] Repeated comparisons have a stable descriptor count and valid packed assets survive save/reopen.
- [ ] Existing native regression checks pass; record upstream provenance and a reproducible failing-before/passing-after receipt.

## Parent

[Feature spec](../spec.md). This draft does not modify the existing native-build parent.

Upstream provenance: [e60138117caaafc0f9b422e5e3f1654127e4287a](https://github.com/blender/blender/commit/e60138117caaafc0f9b422e5e3f1654127e4287a).

## Comments

2026-10-14: Maintainer approved specification, testing seams, ticket granularity and blocking edges. Promoted to ready-for-agent; no implementation performed.

2026-10-14: Added the upstream descriptor close after every comparison loop exit. Native CTest regression reproduced growth from 4 to 104 descriptors over 100 equal comparisons before the fix; it passes afterward. Equal, different and missing statuses pass. Pack/save/reopen also passes through the embedded Python CLI. Full arm64 Blender build passes. CTest results: `dynlib_native`, `python_legacy`, `tiff_native`, `packed_file_cleanup` and `packed_file_roundtrip` pass; unrelated `ghost_native` fails because the host cannot create a valid Cocoa window (`_LSModifyNotification` failure). Interactive file-browser verification remains unverified. Upstream: `e60138117caaafc0f9b422e5e3f1654127e4287a`.

2026-10-14: Review follow-up. Repro command: `ctest --test-dir /private/tmp/retrofit-ticket01-build --output-on-failure -R packed_file_cleanup`; pre-fix source (before `close(file)`) reported `packed-file descriptor count grew from 4 to 104` after 100 equal comparisons, and fixed source passes with a stable count. Added a short-file case which returns `PF_DIFFERS` through the existing stat-size check; an actual read interrupted between `stat` and `read` is not deterministically injectable through this API and remains unverified. Python 2.5 zlib dependency repair and ADR were isolated in commit `f696c188e52`.
