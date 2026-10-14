# 01: Capture native workload profiles and output oracles

**What to build:** Maintainers can reproduce where the optimized native app spends time and compare later experiments against frozen native outputs and budgets.

**Blocked by:** None (can start immediately)

**Status:** ready-for-agent

Approved by the maintainer on 2026-10-14. Blocked tickets remain non-executable until every linked prerequisite has a successful completion receipt. A no-go prerequisite does not unblock dependent integration.

- [ ] Capture representative text reading, editing, image conversion/filtering, compositing, geometry transforms and foreground/background rendering workloads.
- [ ] Record input hashes, seeds/settings, thread counts, build/compiler/host context, timing distributions, attributable CPU cost, allocations and peak memory.
- [ ] Freeze decoded byte outputs and named scene/pass or geometry expectations before evaluating changes; no arbitrary float tolerance or post-result widening.
- [ ] Fix practical gain and regression budgets per selected workload based on observed noise, memory and interactive latency, before candidate measurements.
- [ ] Distinguish native local regression evidence from the still-open original reference corpus; no reference-parity or future speed claim.
- [ ] Reuse existing executable/Python/CTest seams; deliver runnable workload commands and the measured selection or no-hotspot decision.

## Parent

[Feature spec](../spec.md). This draft does not modify the existing native-build parent.

## Comments

2026-10-14: Maintainer approved specification, testing seams, ticket granularity and blocking edges. Promoted to ready-for-agent; no implementation performed.
