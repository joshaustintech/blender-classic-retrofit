# 02: Use safe literal-path touch and link actions

**What to build:** Artists use the original file-browser touch and link actions with literal filenames, preserved link types and controlled helper failures.

**Blocked by:** [safe-rust-safety/01: Prove a safe literal-path touch boundary](01-safe-boundary-probe.md)

**Status:** ready-for-agent

Approved by the maintainer on 2026-10-14. Blocked tickets remain non-executable until every linked prerequisite has a successful completion receipt. A no-go prerequisite does not unblock dependent integration.

- [ ] Proceed only when the boundary probe demonstrates a viable safe contract; no unsafe or shell fallback.
- [ ] Touch preserves file contents and intended metadata; link behavior matches the legacy type, overwrite policy and errors.
- [ ] Exercise original browser actions on byte-preserving names, spaces/quotes/dollar/backticks, symlinks, existing targets and permission failures.
- [ ] Helper failure/cancellation leaves user data intact and uses the original status/UI behavior.
- [ ] The actual packaged app includes the helper and passes native plus browser workflow checks; record any residual C boundary risk.

## Parent

[Feature spec](../spec.md). This draft does not modify the existing native-build parent.

## Comments

2026-10-14: Maintainer approved specification, testing seams, ticket granularity and blocking edges. Promoted to ready-for-agent; no implementation performed.
