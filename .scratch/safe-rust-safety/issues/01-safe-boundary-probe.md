# 01: Prove a safe literal-path touch boundary

**What to build:** A bundled safe Rust prototype performs a real touch operation on a temporary path, with bounded IO and legacy-compatible success/failure behavior.

**Blocked by:** None (can start immediately)

**Status:** ready-for-agent

Approved by the maintainer on 2026-10-14. Blocked tickets remain non-executable until every linked prerequisite has a successful completion receipt. A no-go prerequisite does not unblock dependent integration.

- [ ] Use stable stdlib Rust with mechanically enforced prohibition of authored unsafe code and unsafe export attributes; no added crates.
- [ ] Perform one complete touch request through a minimal owned-message IO boundary and verify unchanged content, literal path bytes and result/error mapping against C.
- [ ] Test malformed/truncated/oversized requests, non-UTF8 names, shell characters, missing permissions, absent/crashed helper and interruption.
- [ ] Record startup/transfer/allocation and cancellation cost; demonstrate packaging on native arm64 without developer-installed Rust.
- [ ] Freeze only the contract needed by this operation and a reusable bounded bulk-payload extension; no speculative generic service.
- [ ] Keep production behavior unchanged during the probe; if no policy-compatible viable boundary exists, complete with a no-go receipt and leave dependent production migrations blocked.

## Parent

[Feature spec](../spec.md). This draft does not modify the existing native-build parent.

## Comments

2026-10-14: Maintainer approved specification, testing seams, ticket granularity and blocking edges. Promoted to ready-for-agent; no implementation performed.
