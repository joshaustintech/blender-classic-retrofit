# 01: Capture native workload profiles and output oracles

**What to build:** Maintainers can reproduce where the optimized native app spends time and compare later experiments against frozen native outputs and budgets.

**Blocked by:** None (can start immediately)

**Status:** ready-for-agent
**Category:** enhancement

Approved by the maintainer on 2026-10-14. Blocked tickets remain non-executable until every linked prerequisite has a successful completion receipt. A no-go prerequisite does not unblock dependent integration.

- [x] Capture representative text reading/editing, image conversion and pixel sampling, geometry transforms, and foreground/background rendering workloads. Record image filtering/scaling and compositor profiling as no-go where no repeatable filter fixture/API or compositor graph fixture/API exists.
- [x] Record input hashes, seeds/settings, thread counts, build/compiler/host context, operation CPU timing distributions and process peak RSS; record Instruments attribution/allocation limits.
- [x] Freeze decoded byte outputs and named scene/pass or geometry expectations before evaluating changes; no arbitrary float tolerance or post-result widening.
- [x] Fix practical gain and regression budgets per selected workload based on observed run-to-run noise, memory and interactive latency before candidate measurements.
- [x] Distinguish native local regression evidence from the still-open original reference corpus; no reference-parity or future speed claim.
- [x] Reuse existing executable/Python/CTest seams; deliver runnable workload commands, measured text operations, candidate dispositions and no-go decisions.

## Parent

[Feature spec](../spec.md). This draft does not modify the existing native-build parent.

## Comments

2026-10-14: Maintainer approved specification, testing seams, ticket granularity and blocking edges. Promoted to ready-for-agent; no implementation performed.

2026-10-14: Completed native workload baseline and frozen oracles in [evidence](../../../docs/evidence/ticket-09-performance-baseline.md), with the Blender 2.45 harness and image fixture under `tools/macos/tests/`. Three optimized Release passes measured text, PNG decode/encode and pixel access, geometry transforms, and foreground/background rendering. Ten native F12 renders gave a displayed foreground completion time of 0.25 s (10 ms UI precision); the budget is 0.26 s P95 for the same scene. `Text.asLines` was profiled as a whole-list workload, not selected as a cursor optimization; Ticket 02 found no repeated sequential-read API to optimize. Compositor profiling, image filtering/scaling, and allocation counts are explicit no-go under [ADR 0008](../../../docs/adr/0008-bound-native-performance-profile-scope.md): no compositor graph/API or repeatable filter fixture/API is available; SIP prevents Instruments Allocations attachment. Foreground output was rendered from a directly opened `.blend` in the optimized native app. Release build passed and CTest passed 12/12. Original Intel reference parity remains separate and unclaimed.
