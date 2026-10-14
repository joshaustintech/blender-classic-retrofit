# Post-2.45 maintenance and safe Rust roadmap

Status: ready-for-agent

Approved by the maintainer on 2026-10-14. Five specs and 25 independent ticket files are tracked locally and ready-for-agent. Approval covers planning/test seams; implementation and release work have not started. No existing parent issue is modified.

## Specs

- [post-245-fixes](../post-245-fixes/spec.md)
- [post-245-performance](../post-245-performance/spec.md)
- [safe-rust-safety](../safe-rust-safety/spec.md)
- [safe-rust-performance](../safe-rust-performance/spec.md)
- [safe-rust-simd](../safe-rust-simd/spec.md)

## Approved testing seams

1. Existing native CTest and Python 2.5 application entry points for files, names, scripts, resource errors and source-verified bug reproductions.
2. Existing packaged-app workflows for actual file-browser actions, painting, rendering, save/reopen and fullscreen; UI evidence is distinct from build results.
3. Frozen native decoded-output/geometry/scene-pass comparisons for local numeric regressions and optimized-release profiling. The original reference corpus remains separately open.
4. One new owned-message helper boundary, demonstrated first by literal-path touch; safe Rust kernels use owned data behind that boundary. No independent per-kernel FFI layer.

No blanket numerical tolerance is proposed. Corrected bug cases have explicit pre-fix expectations; unaffected output stays fixed. Stable stdlib safe Rust is the default; unsafe bridges, third-party crates and nightly toolchains are not approved by this plan.

## Approved ticket breakdown

1. **[post-245-fixes/01: Keep repeated packed-file checks resource-stable](../post-245-fixes/issues/01-packed-file-cleanup.md)** — Blocked by: None. Delivers: Artists can repeatedly compare packed assets against disk files without leaking descriptors or changing comparison results.
2. **[post-245-fixes/02: Preserve names at the legacy length boundary](../post-245-fixes/issues/02-name-boundary.md)** — Blocked by: None. Delivers: Artists can name, duplicate, save and reopen objects at the legacy name limit without reading unterminated data or changing normal suffix rules.
3. **[post-245-fixes/03: Open projects with invalid fonts without crashing](../post-245-fixes/issues/03-font-errors.md)** — Blocked by: None. Delivers: Artists receive the existing controlled failure for missing or malformed packed fonts while valid text shapes and metrics remain unchanged.
4. **[post-245-fixes/04: Keep Python errors safe across project reloads](../post-245-fixes/issues/04-traceback-lifetime.md)** — Blocked by: None. Delivers: Script authors can raise exceptions, load another project and raise another exception without retained traceback references dereferencing stale Blender objects.
5. **[post-245-fixes/05: Compute the legacy quaternion dot product correctly](../post-245-fixes/issues/05-quaternion-dot.md)** — Blocked by: None. Delivers: Existing Python scripts receive the dot product of both operands instead of the first operand multiplied by itself.
6. **[post-245-fixes/06: Keep uniform vertex colors stable under repeated blur](../post-245-fixes/issues/06-paint-rounding.md)** — Blocked by: None. Delivers: Artists can blur a uniformly colored mesh repeatedly without the division approximation darkening its vertex colors.
7. **[post-245-fixes/07: Handle out-of-range depth conversion deterministically](../post-245-fixes/issues/07-depth-conversion.md)** — Blocked by: None. Delivers: Artists can render extreme depth scenes without platform-dependent out-of-range float-to-integer conversion artifacts.
8. **[post-245-fixes/08: Resolve applicability of remaining historical fixes](../post-245-fixes/issues/08-historical-applicability.md)** — Blocked by: None. Delivers: Maintainers receive a source-backed apply/defer/reject decision for every remaining researched historical correctness item without importing later features.
9. **[post-245-performance/01: Capture native workload profiles and output oracles](../post-245-performance/issues/01-profile-and-oracles.md)** — Blocked by: None. Delivers: Maintainers can reproduce where the optimized native app spends time and compare later experiments against frozen native outputs and budgets.
10. **[post-245-performance/02: Read long text blocks without repeated traversal](../post-245-performance/issues/02-text-traversal.md)** — Blocked by: post-245-performance/01. Delivers: Script authors can sequentially read long text blocks efficiently while edits, reset and EOF retain the original Python behavior.
11. **[post-245-performance/03: Qualify one bounded upstream render optimization](../post-245-performance/issues/03-render-backport-qualification.md)** — Blocked by: post-245-performance/01. Delivers: Maintainers receive an implementable small render optimization proposal, or a documented no-go, based on actual cost and unchanged output contracts.
12. **[safe-rust-safety/01: Prove a safe literal-path touch boundary](../safe-rust-safety/issues/01-safe-boundary-probe.md)** — Blocked by: None. Delivers: A bundled safe Rust prototype performs a real touch operation on a temporary path, with bounded IO and legacy-compatible success/failure behavior.
13. **[safe-rust-safety/02: Use safe literal-path touch and link actions](../safe-rust-safety/issues/02-touch-and-links.md)** — Blocked by: safe-rust-safety/01. Delivers: Artists use the original file-browser touch and link actions with literal filenames, preserved link types and controlled helper failures.
14. **[safe-rust-safety/03: Copy assets safely with legacy file-browser behavior](../safe-rust-safety/issues/03-copy-actions.md)** — Blocked by: safe-rust-safety/01. Delivers: Artists copy files and directories through the original browser without shell interpretation or changed overwrite, permission and symlink behavior.
15. **[safe-rust-safety/04: Move assets safely across storage boundaries](../safe-rust-safety/issues/04-move-actions.md)** — Blocked by: safe-rust-safety/01, safe-rust-safety/03. Delivers: Artists move assets using the original browser with literal names and correct same-filesystem and cross-filesystem behavior.
16. **[safe-rust-safety/05: Delete assets safely using existing confirmations](../safe-rust-safety/issues/05-delete-actions.md)** — Blocked by: safe-rust-safety/01. Delivers: Artists delete files, empty directories or directory trees through the original browser without shell expansion and with preserved confirmations and errors.
17. **[safe-rust-safety/06: Qualify safe blend framing through actual open workflows](../safe-rust-safety/issues/06-blend-framing.md)** — Blocked by: safe-rust-safety/01. Delivers: Artists can open valid legacy projects while a safe owned-byte framing stage rejects specified malformed headers and lengths before unsafe reconstruction.
18. **[safe-rust-safety/07: Qualify an exact owned legacy RNG state](../safe-rust-safety/issues/07-rng-qualification.md)** — Blocked by: None. Delivers: Maintainers can reproduce the legacy random sequence with owned safe Rust state and decide whether safe integration preserves sampling and is practical.
19. **[safe-rust-performance/01: Compare safe owned-buffer channel conversion with C](../safe-rust-performance/issues/01-rgba-oracle.md)** — Blocked by: post-245-performance/01. Delivers: Maintainers can run an exact safe Rust RGBA/ABGR operation against the optimized native C oracle and see realistic kernel and transfer costs.
20. **[safe-rust-performance/02: Integrate faster safe batch channel conversion when proven](../safe-rust-performance/issues/02-rgba-integration.md)** — Blocked by: safe-rust-performance/01, safe-rust-safety/01. Delivers: Artists receive the same channel-conversion results faster in the actual packaged app if the complete safe batch operation beats the native baseline.
21. **[safe-rust-performance/03: Qualify an exact safe scalar alpha operation](../safe-rust-performance/issues/03-alpha-oracle.md)** — Blocked by: post-245-performance/01. Delivers: Maintainers can compare one profiled legacy alpha operation with an owned safe Rust scalar oracle before considering vectorization or production replacement.
22. **[safe-rust-simd/01: Demonstrate exact safe SIMD for channel conversion](../safe-rust-simd/issues/01-integer-simd.md)** — Blocked by: safe-rust-performance/02. Delivers: Artists get identical pixel values while a qualified batch path uses demonstrated SIMD only where it improves total operation cost.
23. **[safe-rust-simd/02: Evaluate safe SIMD alpha lanes with fixed arithmetic](../safe-rust-simd/issues/02-alpha-simd.md)** — Blocked by: safe-rust-performance/03, safe-rust-safety/01. Delivers: Artists retain faithful alpha/HDR output while an independent-pixel SIMD alpha candidate is accepted only if it matches the scalar oracle and improves full operation cost.
24. **[safe-rust-simd/03: Evaluate faithful safe batched vertex transforms](../safe-rust-simd/issues/03-batched-transforms.md)** — Blocked by: post-245-performance/01, safe-rust-safety/01. Delivers: Maintainers can demonstrate whether batching an existing transform in safe Rust preserves geometry while improving real calculation cost.
25. **[safe-rust-simd/04: Bound future filter and ray calculation experiments](../safe-rust-simd/issues/04-renderer-followups.md)** — Blocked by: post-245-performance/01. Delivers: Maintainers receive small evidence-backed follow-up specs for costly filter/ray work, or explicit deferrals, without replacing Blender Internal.

## Approval and execution rules

The maintainer approved testing seams, ticket granularity and blocking edges on 2026-10-14. All five specs and 25 tickets have been promoted to ready-for-agent without altering dependencies.

The initial frontier comprises all eight post-2.45 correctness tickets, the native profiling/oracle ticket, the safe touch-boundary probe and the exact RNG qualification ticket: 11 tickets. Work on other tickets begins only after their linked prerequisites have successful completion receipts. A substantiated no-go is valid for a qualification ticket but does not satisfy an integration prerequisite.

Completion receipts go under Comments and include commands/artifacts, corrected versus preserved outputs, real UI evidence where relevant, measured timing distributions and limitations. This approval does not authorize unsafe Rust, new dependencies, nightly toolchains or relaxed fidelity limits. No implementation or release has been performed.

## Deferred research coverage

Historical bugs absent from this baseline, later renderer/features, QMC, approximate AO, new particles/texture nodes, GLSL and Game Engine gains are audit/defer items. Broad instancing/BVH/shading/SSS/simulation migrations require their own follow-up specs after profiling, policy and fidelity gates. Preserve unresolved first-shipped release/archive evidence rather than presenting trunk ancestry as proof.


## Comments

2026-10-14: Maintainer replied “Approved” to the complete five-spec, 25-ticket breakdown and testing seams. Promotion complete.
