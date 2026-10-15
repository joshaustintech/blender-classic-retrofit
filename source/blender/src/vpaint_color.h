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
#ifndef VP_COLOR_H
#define VP_COLOR_H

unsigned int mcol_blend(unsigned int col1, unsigned int col2, int fac);
unsigned int mcol_add(unsigned int col1, unsigned int col2, int fac);
unsigned int mcol_sub(unsigned int col1, unsigned int col2, int fac);
unsigned int mcol_mul(unsigned int col1, unsigned int col2, int fac);
unsigned int mcol_lighten(unsigned int col1, unsigned int col2, int fac);
unsigned int mcol_darken(unsigned int col1, unsigned int col2, int fac);

#endif
