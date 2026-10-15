# ADR 0010: Do not select an unprofiled render backport

Date: 2026-10-14

## Status

Accepted

## Context

Ticket 03 asks for one bounded Blender Internal performance backport or a documented no-go. Ticket 09 profiles the bundled 17-face preview scene and records correct native foreground/background output, but optimized Release symbols were not attributable to Blender functions. The scene has no vector pass, animation workload, large mesh, or radiosity workload.

The adjacent-frame vector conversion path exists and is already gated on requested vector passes, but no measured fixture exercises it. `VlakRen` contains radiosity-only face data, but the profile has no large-scene memory sample to show its impact. The source has per-face/tile and shadow BSP bounds, not a measured object-level frustum-culling hotspot.

## Decision

Select no renderer optimization. Do not infer a target, benefit, or regression budget from upstream anecdotes or the tiny preview render. Record the source applicability and evidence limits in [Ticket 03 qualification](../evidence/ticket-10-render-backport-qualification.md).

## Consequences

Blender Internal rendering code and output remain unchanged. A future bounded proposal needs a representative workload for the candidate path, a native profile identifying measurable cost, and frozen pass/radiosity/animation output oracles before implementation.
