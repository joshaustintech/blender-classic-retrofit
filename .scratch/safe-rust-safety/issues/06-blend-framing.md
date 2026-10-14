# 06: Qualify safe blend framing through actual open workflows

**What to build:** Artists can open valid legacy projects while a safe owned-byte framing stage rejects specified malformed headers and lengths before unsafe reconstruction.

**Blocked by:** [safe-rust-safety/01: Prove a safe literal-path touch boundary](01-safe-boundary-probe.md)

**Status:** ready-for-agent

Approved by the maintainer on 2026-10-14. Blocked tickets remain non-executable until every linked prerequisite has a successful completion receipt. A no-go prerequisite does not unblock dependent integration.

- [ ] Use explicit endian and 32/64-bit decoding, checked offsets/lengths and numeric stored-address IDs in safe stdlib Rust.
- [ ] Preserve compressed input through the existing decompression boundary; do not silently drop compressed files or add a crate to satisfy this ticket.
- [ ] Test partial ENDB, truncated blocks, negative/overflowing lengths, malformed replies and bounded resource use alongside valid cross-endian/pointer-width fixtures.
- [ ] Exercise actual open, linked assets/libraries and save/reopen; compare semantic content and unchanged errors for valid fixtures.
- [ ] Measure the extra transfer/validation cost and freeze accepted limits before live enablement; no-go is valid if safe integration rejects supported files or cannot meet the contract.
- [ ] Explicitly state that DNA reconstruction/object graph relinking remain C; the result is framing validation, not a wholly safe loader.

## Parent

[Feature spec](../spec.md). This draft does not modify the existing native-build parent.

## Comments

2026-10-14: Maintainer approved specification, testing seams, ticket granularity and blocking edges. Promoted to ready-for-agent; no implementation performed.
