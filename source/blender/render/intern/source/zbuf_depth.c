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

#include <limits.h>
#include <math.h>

#include "zbuf_depth.h"

int zbuf_depth_to_int(double depth)
{
	if(isnan(depth) || depth >= (double)INT_MAX) return INT_MAX;
	if(depth <= (double)INT_MIN) return INT_MIN;
	return (int)depth;
}
