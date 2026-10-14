# 03: Copy assets safely with legacy file-browser behavior

**What to build:** Artists copy files and directories through the original browser without shell interpretation or changed overwrite, permission and symlink behavior.

**Blocked by:** [safe-rust-safety/01: Prove a safe literal-path touch boundary](01-safe-boundary-probe.md)

**Status:** ready-for-agent

Approved by the maintainer on 2026-10-14. Blocked tickets remain non-executable until every linked prerequisite has a successful completion receipt. A no-go prerequisite does not unblock dependent integration.

- [ ] Proceed only after a viable boundary probe; implement the narrow copy request without a generic filesystem rewrite.
- [ ] Preserve recursive copy behavior, metadata/permissions, symlink treatment, target-directory interpretation and original return/error conventions.
- [ ] Test nested/empty directories, non-UTF8 and shell-character names, existing targets, partial IO errors, permission failures and interruption.
- [ ] Preserve existing overwrite decisions and progress/cancellation; do not claim atomic copying where legacy semantics are not atomic.
- [ ] Run actual browser copy and downstream scene asset reload in the packaged app; retain data integrity and native checks.

## Parent

[Feature spec](../spec.md). This draft does not modify the existing native-build parent.

## Comments

2026-10-14: Maintainer approved specification, testing seams, ticket granularity and blocking edges. Promoted to ready-for-agent; no implementation performed.
