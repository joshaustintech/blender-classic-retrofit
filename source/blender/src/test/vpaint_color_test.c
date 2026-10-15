/**
 * $Id$
 *
 * ***** BEGIN GPL/BL DUAL LICENSE BLOCK *****
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; either version 2
 * of the License, or (at your option) any later version. The Blender
 * Foundation also sells licenses for use in proprietary software under
 * the Blender License. See http://www.blender.org/BL/ for information.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU General Public License for more details.
 *
 * ***** END GPL/BL DUAL LICENSE BLOCK *****
 */

#include <stdio.h>

#include "../vpaint_color.h"

#define COLOR(c0, c1, c2, c3) (((unsigned int)(c3) << 24) | ((unsigned int)(c2) << 16) | ((unsigned int)(c1) << 8) | (c0))

static int expect_color(const char *operation, unsigned int actual,
						unsigned int expected)
{
	if(actual == expected) return 1;
	fprintf(stderr, "%s: expected 0x%08x, got 0x%08x\n",
			operation, expected, actual);
	return 0;
}

int main(void)
{
	unsigned int color= COLOR(7, 80, 100, 127);
	unsigned int repeated= color;
	int i, passed= 1;

	passed &= expect_color("blend factor zero", mcol_blend(color, 0, 0), color);
	passed &= expect_color("blend factor full", mcol_blend(color, 0, 255), 0);
	passed &= expect_color("blend byte extrema",
					   mcol_blend(COLOR(0, 128, 255, 127),
								  COLOR(0, 128, 255, 127), 127),
					   COLOR(255, 128, 255, 127));
	for(i= 0; i < 100; i++)
		repeated= mcol_blend(repeated, color, 127);
	passed &= expect_color("repeated uniform blend",
					   repeated, COLOR(255, 80, 100, 127));
	passed &= expect_color("intermediate blend",
					   mcol_blend(COLOR(0, 20, 50, 80),
								  COLOR(0, 40, 60, 100), 127),
					   COLOR(255, 29, 54, 89));
	passed &= expect_color("intermediate add",
					   mcol_add(COLOR(0, 20, 50, 80),
							COLOR(0, 40, 30, 10), 128),
					   COLOR(255, 40, 65, 85));
	passed &= expect_color("add factor zero", mcol_add(color, 0, 0), color);
	passed &= expect_color("add factor full",
					   mcol_add(COLOR(0, 20, 50, 80),
							COLOR(0, 40, 30, 10), 255),
					   COLOR(255, 60, 80, 90));
	passed &= expect_color("add saturation boundary",
					   mcol_add(COLOR(0, 127, 127, 127),
							COLOR(0, 127, 127, 127), 255),
					   COLOR(255, 254, 254, 254));
	passed &= expect_color("intermediate subtract",
					   mcol_sub(COLOR(0, 40, 65, 105),
							COLOR(0, 20, 15, 5), 128),
					   COLOR(255, 30, 58, 103));
	passed &= expect_color("subtract factor zero", mcol_sub(color, 0, 0), color);
	passed &= expect_color("subtract factor full",
					   mcol_sub(COLOR(0, 40, 65, 105),
							COLOR(0, 20, 15, 5), 255),
					   COLOR(255, 20, 50, 100));
	passed &= expect_color("subtract floor boundary",
					   mcol_sub(COLOR(0, 20, 50, 100),
							COLOR(0, 127, 127, 127), 255),
					   COLOR(255, 0, 0, 0));
	passed &= expect_color("intermediate multiply",
					   mcol_mul(COLOR(0, 100, 50, 20),
							COLOR(0, 128, 128, 128), 127),
					   COLOR(255, 75, 37, 15));
	passed &= expect_color("multiply factor zero", mcol_mul(color, 0, 0), color);
	passed &= expect_color("multiply factor full",
					   mcol_mul(COLOR(0, 100, 50, 20),
							COLOR(0, 128, 128, 128), 255),
					   COLOR(255, 50, 25, 10));
	passed &= expect_color("intermediate lighten",
					   mcol_lighten(COLOR(0, 20, 30, 40),
								COLOR(0, 40, 50, 60), 127),
					   COLOR(255, 29, 39, 49));
	passed &= expect_color("lighten unchanged darker input",
					   mcol_lighten(COLOR(0, 40, 50, 60),
								COLOR(0, 20, 30, 40), 127),
					   COLOR(0, 40, 50, 60));
	passed &= expect_color("intermediate darken",
					   mcol_darken(COLOR(0, 40, 50, 60),
								COLOR(0, 20, 30, 40), 127),
					   COLOR(255, 30, 40, 50));
	passed &= expect_color("darken unchanged brighter input",
					   mcol_darken(COLOR(0, 20, 30, 40),
								COLOR(0, 40, 50, 60), 127),
					   COLOR(0, 20, 30, 40));
	passed &= expect_color("lighten factor zero", mcol_lighten(color, 0, 0), color);
	passed &= expect_color("lighten factor full", mcol_lighten(color, 0, 255), 0);
	passed &= expect_color("darken factor zero", mcol_darken(color, 0, 0), color);
	passed &= expect_color("darken factor full", mcol_darken(color, 0, 255), 0);

	return passed ? 0 : 1;
}
