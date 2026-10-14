# Measure render parity with fixed numerical tolerances

Rendering comparisons use identical scenes and settings, exact decoded-pixel matches where achievable, and strict per-scene tolerances established from reference measurements before porting. This was chosen over requiring exact decoded pixels everywhere to permit measured floating-point differences between Intel and Apple Silicon without changing shading, render defaults, or visible output. Tolerances cannot be widened retrospectively to make a regression pass.
