# 0007: Saturate exceptional integer z-buffer depth conversions

**Status:** Accepted

**Date:** 2026-10-14

Blender 2.45 rasterizers interpolate depth as `double` and convert it to signed `int`. Converting a finite value outside the representable range, infinity, or NaN is not portable C behavior. Upstream commit `2009ef810e5317cdbbf31caa36decb17947e225f` adds saturation for range overflow but does not define NaN behavior.

Use one conversion boundary for integer z-buffer paths:

- Finite values at or beyond integer limits saturate to `INT_MIN` or `INT_MAX`.
- Values within range retain C's truncation toward zero.
- Positive and negative infinity saturate to their respective limits.
- NaN maps to `INT_MAX`, the existing far-depth/background sentinel.

This defines deterministic handling at the conversion boundary while preserving ordinary depth results. It does not change depth interpolation, clipping, masks, render defaults, or stored floating-point transparency samples. Focused tests cover the boundary and exceptional inputs; render fixtures remain necessary to assess changes to extreme scenes.

**Consequences:** NaN pixels behave as far/background depth in the affected integer z-buffer paths. This policy is local to those casts and does not claim to make other renderer NaN handling deterministic.
