# Ticket 09: native performance baseline

Date: 2026-10-14

## Workloads and frozen outputs

Three independent optimized-build passes used Blender's bundled `release/datafiles/preview.blend` (SHA-256 `5dc7697f00d1ab5f88c738cac5507e61ef179a2662cddf56028463ae21891fce`) and fixed image/text inputs. The scene stores a 600×600 base resolution and 25% resolution percentage, yielding a 150×150 rendered PNG; the harness checks the PNG IHDR dimensions on each run. Host was an arm64 MacBook Pro with 16 GiB RAM, macOS 27.0.1, Apple clang 21.0.0, Xcode 27.0; optimized CMake build used `Release`, `-O3 -DNDEBUG`, arm64, Game Engine/player off, Python 2.5.6. There were no random inputs. Blender/Python workloads were single-process; render CLI used `-t 1`.

| Workload | Samples per pass | Median seconds, passes 1 / 2 / 3 | P95 seconds, passes 1 / 2 / 3 | Peak RSS range across passes | Frozen oracle |
| --- | ---: | ---: | ---: | ---: | --- |
| `Text.asLines`, 50,000 lines | 12 | 0.007566 / 0.006661 / 0.007369 | 0.008188 / 0.008147 / 0.008721 | 54.22–60.67 MiB | text SHA-256 `3b56fb5a1a356e1e88054e06712ea09f3cbac3bac1893275af66b0109a13c63a`; line count and first/last lines asserted |
| `Text.write`, 2,199,999 bytes | 10 | 1.148648 / 1.149548 / 1.151018 | 1.380950 / 1.192392 / 1.500657 | 50.22–50.92 MiB | line count and first/last lines asserted; undo-limit warnings explain long-tail samples |
| PNG decode + encode, 2048² | 8 | 0.389578 / 0.387410 / 0.387728 | 0.396764 / 0.397010 / 0.391474 | 161.15–161.37 MiB | fixture/output PNG SHA-256 `10c685ddc76b4bba232c0490899485a501c037d5c9dbbdce11b61a144a434866` |
| PNG integer-pixel sampling, 128² grid | 100 | 0.004840 / 0.004839 / 0.004845 | 0.004876 / 0.004913 / 0.004912 | 47.90–48.08 MiB | weighted pixel checksum `45080011` |
| 16,384-vertex / 16,129-face mesh transform | 1,200 | 0.000119 / 0.000120 / 0.000119 | 0.000121 / 0.000121 / 0.000120 | 26.80–27.08 MiB | first/last vertex and topology SHA-256 `9749f96ce8032a6c3e511374d816ded2e4bca711047b75cda49e5b73e52718d8` |
| Background render of `preview`, frame 1, 150² output, OSA 5, PNG, one thread | 8 separate renders | 0.391214 / 0.410887 / 0.362459 | 0.515612 / 0.500245 / 0.500017 | 21.61–21.99 MiB per render process | PNG SHA-256 `bdae612d5ed0d4f9384ab8b1aab50d135e12995fd5855cbe343f2351bb8dd9df`; decoded RGBA8 premultiplied-sRGB SHA-256 `5c66a26e7096fce7d3d0e134643d8399bad81f3db036d2c630e3dbba2c751ac0` |

The Release GUI opened the same preview file directly as the Blender executable's file argument. Ten native F12 runs rendered the same preview as background rendering; each completed render showed `Time:00:00.25` in Blender's main header, quantized to hundredths of a second. This gives a foreground completion baseline of 0.25 s at the UI's displayed precision, with a practical P95 ceiling of 0.26 s for repeat comparisons. It is a render completion measurement, not input-to-first-feedback latency. The full native-app render is captured at [ticket-09-foreground-render.png](ticket-09-foreground-render.png).

`/usr/bin/time -l` supplied wall/user/system time, maximum RSS, and peak footprint. Peak RSS is a process high-water mark, not a per-operation allocation count. Instruments Time Profiler sampled the direct Release Blender process at 1 ms for 1.58 s and recorded 295 sample rows; Release symbols do not resolve to attributable Blender functions, so this yielded no defensible hotspot ranking. Instruments Allocations refused attachment: “Target process is marked restricted and cannot be traced while System Integrity Protection is enabled.” No allocation-count claim is made.

## Frozen comparison budget and candidate selection

For `Text.asLines`, the three process medians span 0.000905 s (12.0% of the first-pass median); P95 spans 0.000574 s, and peak RSS spans 6.45 MiB. A future candidate must beat these same-workload run-to-run spreads, keep P95 at or below 0.008721 s, preserve the text oracle, and keep maximum RSS within the observed baseline maximum plus 6.45 MiB. These budgets are specific to this fixture and host; no universal speed target is asserted. Non-render medians for the other operations varied by 0.12–0.84% between their recorded passes; background-render medians spanned 0.048428 s (12.4%), reflecting separate-process launch and scheduling noise. Compare candidates against the same three-pass baseline and use the per-workload median span as the minimum gain. Preserve exact output hashes/checksums and do not exceed the worst observed P95 or maximum RSS plus that workload's measured RSS range. Rendering is retained as a correctness and regression workload, not selected as an optimization target.

No implementation candidate was selected from this profile. `Text.asLines` is a whole-list snapshot that walks from the first line to EOF once per call, so it does not expose the repeated sequential-read traversal in the later 2.48 change. Ticket 02 confirmed that no cursor API exists in this 2.45 wrapper and rejected adding persistent cursor state without bounded invalidation; see [ADR 0009](../adr/0009-no-text-cursor-backport.md). Image I/O consumes most RSS in this sample but is a decode/encode workload, not evidence for an image-scaling change. Geometry transform and the bundled render have no profiled function-level hotspot.

## Coverage limits

The bundled preview scene has no compositor node graph, and Blender 2.45's bundled Python 2.5 API exposes no scene compositor node-tree builder. This repository has no compositor graph fixture. A compositor workload/oracle therefore remains uncaptured; enabling “Do Composite” on this scene would be a no-op and would give false evidence. Image scaling/filter timing is also an explicit no-go: the bundled `Image` Python API has load/save, pixel access, and pack/unpack operations but no resize/filter operation, and the ticket has no deterministic native-editor filter fixture. Image decode/encode conversion and sampled pixel access are measured. Both scope limits are deliberate under [ADR 0008](../adr/0008-bound-native-performance-profile-scope.md).

The original Intel reference release is not represented by these local regression measurements.

## Reproduction

Build the same optimized binary with the repository's documented CMake setup, then prepare the deterministic text and eight image copies:

```sh
python3 - <<'PY'
path = '/private/tmp/ticket09-text.txt'
with open(path, 'wb') as output:
    output.write('\n'.join(
        'line-%06d-name-and-value=0123456789abcdef' % index
        for index in range(50000)).encode('ascii'))
PY
for index in 1 2 3 4 5 6 7 8; do
  cp tools/macos/tests/fixtures/performance-2048.png "/private/tmp/ticket09-image-${index}.png"
done
```

Run each mode three times with the same environment. `render` mode launches eight separate single-threaded background renders and checks every PNG hash. Other modes time each operation within one process and report median/P95/min/max.

```sh
BLENDER=/path/to/blender.app/Contents/MacOS/blender
for pass in 1 2 3; do
  for mode in text-read text-edit image-io image-pixels geometry render; do
    PERF_MODE="$mode" \
    PERF_ROOT="$PWD" \
    PERF_TMP=/private/tmp \
    PERF_BLENDER="$BLENDER" \
      /usr/bin/time -l "$BLENDER" -b release/datafiles/preview.blend \
      -P tools/macos/tests/performance-workload.py
  done
done
```

Validate the decoded render oracle with macOS ImageIO/CoreGraphics and CryptoKit, rendering the scene's 600x600 base resolution at 25% (150x150 output) into an RGBA8 premultiplied-sRGB buffer before hashing. The harness verifies the output PNG is 150x150 before accepting its hash. Run the foreground check by launching `$BLENDER "$PWD/release/datafiles/preview.blend"`, raising that process's native Blender window, and invoking F12. This path does not use file associations.

## Validation

- The behavior-oracle preflight passed before the workload harness runs: 50,000 text lines and first/last values, PNG dimensions and pixel channel reads, active scene camera, Blender 2.45 version.
- The checked-in workload harness passed each mode for all three passes; render PNG and geometry/text/image/pixel oracles stayed identical.
- Optimized Release build passed. CTest passed 12/12, including native GHOST, with WindowServer access.
- No application code changed. Mutation testing is not applicable; this ticket contains a Python 2.5 benchmark harness and fixtures, not Rust code.
