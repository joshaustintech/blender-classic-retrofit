# Qualify safe Rust replacements for risky C boundaries

Status: ready-for-agent

Approved by the maintainer on 2026-10-14: specification, testing seams, ticket granularity and blocking edges. Implementation has not started.

## Problem Statement

Some existing filesystem, binary parsing, pixel and RNG code relies on shared buffers, shell construction, raw pointers or mutable state. Users need safer operations while retaining existing Blender workflows.

## Solution

Prove a strict safe integration boundary with one literal-path operation, then migrate individual filesystem actions only when their behavior matches. Separately qualify file framing and RNG state with exact oracles.

## User Stories

1. As an artist, I want filenames containing spaces and shell characters to be treated literally, so that file actions cannot execute unintended commands.
2. As an artist, I want non-UTF8 filesystem names to remain usable, so that the port does not narrow supported names.
3. As an artist, I want touch actions to preserve file content, so that marking a file does not damage it.
4. As an artist, I want copying files and directories to preserve legacy overwrite behavior, so that file-browser actions remain familiar.
5. As an artist, I want moves across filesystems to retain their semantics, so that moving an asset does not fail just because storage changes.
6. As an artist, I want deletion to preserve file, empty-directory and recursive distinctions, so that existing prompts and controls remain meaningful.
7. As an artist, I want link operations to preserve the original link type, so that asset relationships remain correct.
8. As an artist, I want permissions and symlinks to behave predictably, so that safe code does not create data-loss regressions.
9. As an artist, I want malformed blend framing to be rejected safely, so that opening an invalid file does not corrupt process memory.
10. As an artist, I want valid compressed and cross-architecture files to keep opening, so that safety does not remove supported formats.
11. As an artist, I want cancellation and errors to use existing UI, so that the safe helper does not add new dialogs or controls.
12. As a script author, I want random sequences to remain identical for the same state, so that procedural and render results remain reproducible.
13. As a maintainer, I want authored Rust to be mechanically checked for unsafe code, so that the safety promise can be reviewed.
14. As a maintainer, I want helper failure and malformed replies to fail predictably, so that a new component does not corrupt Blender state.
15. As a maintainer, I want the scope of file validation to be explicit, so that a framing validator is not advertised as a wholly safe loader.
16. As a maintainer, I want unsafe FFI or dependency changes to require a separate decision, so that a safe facade does not silently weaken policy.

## Implementation Decisions

- Preserve feature parity, UX parity, the Python 2.5 API and Blender Internal. Retain original controls, defaults, progress, cancellation and file semantics.
- Keep Game Engine/player excluded. Import no later UI, shader, particle, modifier or renderer feature merely because it accompanies a useful patch.
- Use existing native application tests and packaged-app workflows as the primary acceptance boundary. Add a focused lower-level oracle only for behavior that cannot be observed reliably there.
- Use the research snapshot as provenance, then recheck the actual implementation branch before each change. Carry only the responsible patch and required correctness dependencies.
- Distinguish approved corrected behavior from unchanged behavior. Freeze a narrowly described failing-case expectation before fixing it; preserve unaffected outputs and never widen tolerances afterward.
- Use stable, standard-library-only Rust with a mechanically enforced prohibition on authored unsafe Rust. No unsafe attributes, pointer-to-slice conversion or hidden authored unsafe bridge.
- An owned-buffer helper through ordinary IO is a candidate integration boundary, not an already approved performance solution. Demonstrate one complete operation and its errors/cancellation before expanding it.
- No per-pixel, per-vertex or per-random-number process calls. Include process startup, serialization, buffer copies, IPC, allocation and packaging in total cost.
- If the safe integration boundary cannot preserve behavior or is impractical, retain the C path and finish with a documented no-go result. No forced migration to meet a language target.
- Standard-library/compiler/OS implementation internals remain trusted dependencies; the authored-safe constraint does not mean every dependency contains zero unsafe code.
- Use existing file-browser actions as the observable filesystem seam. Split touch/link, copy, move and deletion into independently verifiable slices.
- Bound requests and replies, preserve path bytes, reject truncated/invalid messages, and map results to existing return/error behavior. Introduce only the operation needed by each slice.
- Prove the boundary with a touch operation in a temporary workspace before enabling it in production. Freeze the resulting contract and failure behavior; do not build a speculative generic service.
- Keep the original C filesystem path available until each corresponding safe operation passes its workflow tests; do not silently invoke a shell as a fallback for unsupported names.
- For blend framing, use owned uncompressed byte records and checked endian/width/length decoding. Preserve existing decompression outside the safe helper unless an independently authorized safe stdlib solution exists. Stored old addresses are numeric IDs.
- Framing validation does not replace DNA reconstruction, object graph relinking or all loader pointer usage. State the remaining C risk explicitly.
- Qualify the exact legacy RNG sequence using explicit wrapping arithmetic; retain the production RNG unless safe integration also preserves stream consumption and cost.

## Testing Decisions

- Test observable results, errors and resource behavior rather than source structure or the presence of a particular optimization.
- Reuse the existing native CTest, embedded Python regression and image-format checks; extend those seams rather than build a parallel test framework.
- Exercise actual packaged startup, editing, save/reopen, foreground/background rendering and fullscreen for changes affecting those workflows. Compilation is not GUI or render-parity proof.
- Keep native-checkout regression evidence separate from original reference-release evidence. The original reference corpus is still an open gate; do not claim reference parity from the native oracle alone.
- Run filesystem actions through the original browser in temporary directories, plus focused path/error cases. Verify content, metadata, prompts, return statuses and absence of unintended side effects.
- Include helper absence/crash, bounded-message exhaustion, invalid length, permission errors and cancellation. Failures must leave original user data intact.
- Validate framing with 32/64-bit, both-endian, compressed/uncompressed, linked-library and partial terminator fixtures; exercise actual open/save/reopen, not only a parser unit test.
- Use exact RNG sequence comparisons and sampling-order fixtures; classify oracle-only evidence separately from app integration.

## Out of Scope

Unsafe Rust, authored unsafe FFI attributes, third-party crates, silent shell fallback, a full safe blend loader, UI redesign, RNG algorithm replacement, and migration of every C pointer at once.

## Further Notes

The helper is a proposed mechanism constrained by the existing safe/std-only policy. Plan approval does not approve a nightly toolchain, dependencies or a weaker FFI policy. Each conditional ticket can terminate with a substantiated no-go receipt.

Research provenance: [post-2.45 research](../../docs/evidence/post-245-backports-and-safe-rust.md). Stable source contracts are described by module behavior rather than implementation file paths.

## Comments

2026-10-14: Maintainer approved specification, testing seams, ticket granularity and blocking edges. Promoted to ready-for-agent; no implementation performed.
