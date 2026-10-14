# Domain Docs

## Layout and reading rules

This repository uses a single context: `CONTEXT.md` at the repository root and decisions under `docs/adr/`. Existing upstream documentation remains in `doc/`.

Before exploring, read `CONTEXT.md` if present and the ADRs relevant to the affected area. If these files are absent, proceed silently. Create domain documentation lazily through `/domain-modeling` when terms or decisions are resolved.

## Vocabulary and decisions

Use the glossary's established terms in issues, tests, and proposals. Reconsider invented synonyms; record genuine glossary gaps for `/domain-modeling`.

Identify any conflict with an existing ADR explicitly, naming the decision and the reason to revisit it.

## Retrofit scope

Preserve Blender 2.45's features and UX. The first milestone is a native macOS Apple Silicon build. See `docs/macos-build-preflight.md` for measured blockers and acceptance boundaries. Later safe, standard-library-only Rust and upstream performance/stability fixes require evidence of compatibility and benefit.
