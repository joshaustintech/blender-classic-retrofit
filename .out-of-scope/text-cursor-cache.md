# Cached Blender Text line cursor

Blender 2.45's Python `Blender.Text` wrapper does not expose sequential line reads. Its `asLines()` method makes a full snapshot with one forward traversal. A cached cursor cannot optimize that contract without adding a new API or changing snapshot behavior.

The native editor, undo/redo, reload, and Python wrapper can replace or mutate the linked list. The current `Text` model has no bounded generation signal that a Python cursor could use to detect every such change. Keeping a `TextLine *` in the Python wrapper risks a stale pointer; adding mutation tracking across those paths is broader than this performance backport and has no profiled payoff.

We therefore rejected the cached-cursor backport and left the application code unchanged. Reconsider a sequential-reader feature only as a separately approved API design with a safe mutation-generation seam and a workload demonstrating a worthwhile native bottleneck. See [ADR 0009](../docs/adr/0009-no-text-cursor-backport.md).

## Prior requests

- `.scratch/post-245-performance/issues/02-text-traversal.md`: cached cursor backport for Blender 2.45.
