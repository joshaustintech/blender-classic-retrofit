# Repository Guidelines

## Project Structure & Module Organization

This repository contains Blender's legacy 2.45 source tree. `source/blender/` holds application modules, `source/creator/` the executable entry point, and `source/gameengine/` the game engine. `intern/` contains internal libraries such as GHOST and guardedalloc; `extern/` contains bundled third-party code. Runtime scripts and assets live in `release/`, translations in `po/`, build helpers in `tools/`, and platform settings in `config/`. Build documentation is in `doc/`; tests sit beside their modules, usually under `test/`.

## Build, Test, and Development Commands

Use a compatible legacy toolchain. The SCons documentation specifies Python 2.4+ and SCons 0.97; the scripts use Python 2 syntax. Consult `doc/blender-scons.txt` or `doc/blender-cmake.txt` before configuring dependencies.

- `scons`: build and stage Blender using platform defaults.
- `scons blendernogame`: build without the game engine.
- `make` / `make debug`: compile using the legacy Makefiles, normally or with debugging enabled.
- CMake alternative, from the repository root:

  ```sh
  mkdir -p ../blender-build
  cd ../blender-build
  cmake -G "Unix Makefiles" ../blender-classic-retrofit
  make
  ./bin/blender
  ```

For SCons, launch the executable from `BF_INSTALLDIR`. Keep local overrides in `user-config.py` or `user-def.mk`, and generated outputs outside tracked source directories. Modern toolchain compatibility requires separate verification.

## Coding Style & Naming Conventions

Match nearby code: C/C++ commonly uses tabs; Python indentation varies by file. Preserve module prefixes such as `BLI_`, `BKE_`, and `GHOST_`, existing header organization, and license notices. Keep compatibility changes focused. No repository-wide formatter or lint configuration is supplied.

## Testing Guidelines

Tests are standalone module programs rather than a unified framework; no coverage threshold is configured. Extend the affected module's existing tests and naming conventions, such as `memtest.c` or `GHOST_Test.cpp`. With the legacy Make environment and libraries configured, build guardedalloc tests with `make -C intern/guardedalloc/test all`, then run `make -C intern/guardedalloc/test test`. Verify changed Blender workflows interactively and record commands, results, and toolchain limitations.

## Commit & Pull Request Guidelines

The available Git history contains one imported branch merge, so it establishes no modern commit convention. Use concise imperative subjects and include Codex co-authorship for agent-authored commits. PRs should explain the change, link relevant issues, describe validation, and include screenshots for visible UI changes.

## Agent Instructions

Use `caveman ultra` by default and `ponytail ultra` for coding work unless instructed otherwise. Prefer simple code and minimal tool requests; keep contributor documentation professional.

## Agent skills

### Issue tracker

Track issues and specs as local Markdown under `.scratch/<feature>/`. See `docs/agents/issue-tracker.md`.

### Triage labels

Use the five default triage roles in each issue's `Status:` field. See `docs/agents/triage-labels.md`.

### Domain docs

Use a single root `CONTEXT.md` and `docs/adr/`. See `docs/agents/domain.md`.
