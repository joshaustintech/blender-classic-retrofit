# 05: Delete assets safely using existing confirmations

**What to build:** Artists delete files, empty directories or directory trees through the original browser without shell expansion and with preserved confirmations and errors.

**Blocked by:** [safe-rust-safety/01: Prove a safe literal-path touch boundary](01-safe-boundary-probe.md)

**Status:** ready-for-agent

Approved by the maintainer on 2026-10-14. Blocked tickets remain non-executable until every linked prerequisite has a successful completion receipt. A no-go prerequisite does not unblock dependent integration.

- [ ] Keep file, empty-directory and recursive deletion distinct; retain original confirmations and controls.
- [ ] Treat arbitrary path bytes literally and match legacy symlink behavior without following unintended targets.
- [ ] Test nested directories, shell-character/non-UTF8 names, missing files, permission errors, cancellation and helper failure in a disposable workspace.
- [ ] Failures do not touch unintended paths; internal cleanup callers retain their existing result contract.
- [ ] Actual packaged browser and temporary-scene/undo cleanup checks pass; record the scope of remaining C operations.

## Parent

[Feature spec](../spec.md). This draft does not modify the existing native-build parent.

## Comments

2026-10-14: Maintainer approved specification, testing seams, ticket granularity and blocking edges. Promoted to ready-for-agent; no implementation performed.
