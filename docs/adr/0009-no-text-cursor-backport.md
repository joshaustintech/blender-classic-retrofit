# ADR 0009: Do not add a cached text cursor without a cursor API

Date: 2026-10-14

## Status

Accepted

## Context

Ticket 02 proposed applying Blender 2.48's cached `TextLine` cursor optimization to Blender 2.45. The checked-out Python `Blender.Text` type exposes `asLines()` as its only line-reading method. `Text_asLines` starts at `text->lines.first`, visits each node once, and creates a fresh Python list and string values. There is no `readline`, iterator, seek, or reset operation whose sequential calls repeat a linked-list walk.

The native text editor, undo/redo, reload, and Python wrapper can all mutate or replace the linked list. A persistent pointer cache would need reliable invalidation from every one of those paths. Adding an incremental read API and its cursor state would be new API behavior rather than a performance-only backport; it is not justified by the measured whole-list operation or the approved scope.

## Decision

Do not add cursor state, a new incremental reading API, or a linked-list cache. Keep `Text_asLines` unchanged. Record the 50,000-line workload as a measurement of whole-list materialization, not evidence of repeated traversal. Ticket 02 is a no-go for a cached-cursor backport. Reconsider only with a separately approved API proposal, mutation-generation seam, and workload showing repeated reads are a worthwhile native bottleneck.

## Consequences

The Python 2.5 API, snapshots, and mutation behavior stay unchanged; no stale `TextLine *` can be retained. The 2.48 speedup is not claimed for this 2.45 code path, and no performance change is delivered by Ticket 02.
