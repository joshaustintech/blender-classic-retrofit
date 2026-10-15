/**
 * $Id$
 *
 * ***** BEGIN GPL/BL DUAL LICENSE BLOCK *****
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; either version 2
 * of the License, or (at your option) any later version. The Blender
 * Foundation also sells licenses for use in proprietary software
 * under the Blender License. See http://www.blender.org/BL/ for information.
 *
 * ***** END GPL/BL DUAL LICENSE BLOCK *****
 */

#include "zbuf_depth.h"

#include <limits.h>
#include <math.h>
#include <stdio.h>

static int expect_depth(const char *name, double depth, int expected)
{
	int actual = zbuf_depth_to_int(depth);
	if (actual == expected) return 0;
	fprintf(stderr, "%s: expected %d, got %d\n", name, expected, actual);
	return 1;
}

int main(void)
{
	int failures = 0;
	failures += expect_depth("ordinary positive", 42.75, 42);
	failures += expect_depth("ordinary negative", -42.75, -42);
	failures += expect_depth("ordinary positive fraction", 0.75, 0);
	failures += expect_depth("ordinary negative fraction", -0.75, 0);
	failures += expect_depth("upper boundary", (double)INT_MAX, INT_MAX);
	failures += expect_depth("upper overflow", (double)INT_MAX + 1024.0, INT_MAX);
	failures += expect_depth("positive infinity", INFINITY, INT_MAX);
	failures += expect_depth("lower boundary", (double)INT_MIN, INT_MIN);
	failures += expect_depth("lower overflow", (double)INT_MIN - 1024.0, INT_MIN);
	failures += expect_depth("negative infinity", -INFINITY, INT_MIN);
	failures += expect_depth("not a number", NAN, INT_MAX);
	return failures != 0;
}
