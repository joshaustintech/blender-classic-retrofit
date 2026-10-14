# 03: Qualify an exact safe scalar alpha operation

**What to build:** Maintainers can compare one profiled legacy alpha operation with an owned safe Rust scalar oracle before considering vectorization or production replacement.

**Blocked by:** [post-245-performance/01: Capture native workload profiles and output oracles](../../post-245-performance/issues/01-profile-and-oracles.md)

**Status:** ready-for-agent

Approved by the maintainer on 2026-10-14. Blocked tickets remain non-executable until every linked prerequisite has a successful completion receipt. A no-go prerequisite does not unblock dependent integration.

- [ ] Select only one profiled existing alpha-over/under/add operation and freeze its scalar expression, alpha rules and width/order.
- [ ] Implement safe owned-buffer scalar Rust without approximate math, fused replacements or a new compositing algorithm.
- [ ] Test HDR, alpha boundaries, exceptional floats, signed zero, subnormals and tails against the frozen native oracle and existing scene/pass limits.
- [ ] Provide representative total-cost estimates and a safe integration contract proposal; keep production defaults unchanged.
- [ ] Reject the candidate if equivalence or worthwhile cost cannot be established; hand an exact oracle to the SIMD experiment only on success.

## Parent

[Feature spec](../spec.md). This draft does not modify the existing native-build parent.

## Comments

2026-10-14: Maintainer approved specification, testing seams, ticket granularity and blocking edges. Promoted to ready-for-agent; no implementation performed.
