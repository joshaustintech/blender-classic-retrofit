# Unprofiled renderer performance backports

Do not import historical Blender Internal optimizations based only on upstream timing anecdotes or source resemblance. This checkout must first exercise the candidate path with a representative scene, measure its native cost, and freeze pass, radiosity, animation, and output oracles. A code change without those measurements would have no defensible gain or regression budget.

Ticket 03 found that the available optimized Release profile uses a 17-face preview scene, does not request vector passes, and cannot attribute samples to Blender functions. The reviewed candidates need vector-pass animation, large radiosity meshes, or a new object-bound culling algorithm. No candidate was selected; see [ADR 0010](../docs/adr/0010-no-unprofiled-render-backport.md) and the [source-cited qualification](../docs/evidence/ticket-10-render-backport-qualification.md).

## Prior requests

- `.scratch/post-245-performance/issues/03-render-backport-qualification.md`: select one bounded post-2.45 renderer performance backport.
