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

#include "vpaint_color.h"

unsigned int mcol_blend(unsigned int col1, unsigned int col2, int fac)
{
	char *cp1, *cp2, *cp;
	int mfac;
	unsigned int col=0;

	if(fac==0) return col1;
	if(fac>=255) return col2;

	mfac= 255-fac;
	cp1= (char *)&col1;
	cp2= (char *)&col2;
	cp=  (char *)&col;

	cp[0]= 255;
	cp[1]= (mfac*cp1[1]+fac*cp2[1])/255;
	cp[2]= (mfac*cp1[2]+fac*cp2[2])/255;
	cp[3]= (mfac*cp1[3]+fac*cp2[3])/255;
	return col;
}

unsigned int mcol_add(unsigned int col1, unsigned int col2, int fac)
{
	char *cp1, *cp2, *cp;
	int temp;
	unsigned int col=0;

	if(fac==0) return col1;

	cp1= (char *)&col1;
	cp2= (char *)&col2;
	cp=  (char *)&col;

	cp[0]= 255;
	temp= cp1[1] + ((fac*cp2[1])/255);
	if(temp>254) cp[1]= 255; else cp[1]= temp;
	temp= cp1[2] + ((fac*cp2[2])/255);
	if(temp>254) cp[2]= 255; else cp[2]= temp;
	temp= cp1[3] + ((fac*cp2[3])/255);
	if(temp>254) cp[3]= 255; else cp[3]= temp;
	return col;
}

unsigned int mcol_sub(unsigned int col1, unsigned int col2, int fac)
{
	char *cp1, *cp2, *cp;
	int temp;
	unsigned int col=0;

	if(fac==0) return col1;

	cp1= (char *)&col1;
	cp2= (char *)&col2;
	cp=  (char *)&col;

	cp[0]= 255;
	temp= cp1[1] - ((fac*cp2[1])/255);
	if(temp<0) cp[1]= 0; else cp[1]= temp;
	temp= cp1[2] - ((fac*cp2[2])/255);
	if(temp<0) cp[2]= 0; else cp[2]= temp;
	temp= cp1[3] - ((fac*cp2[3])/255);
	if(temp<0) cp[3]= 0; else cp[3]= temp;
	return col;
}

unsigned int mcol_mul(unsigned int col1, unsigned int col2, int fac)
{
	char *cp1, *cp2, *cp;
	int mfac;
	unsigned int col=0;

	if(fac==0) return col1;

	mfac= 255-fac;
	cp1= (char *)&col1;
	cp2= (char *)&col2;
	cp=  (char *)&col;

	/* first mul, then blend the fac */
	cp[0]= 255;
	cp[1]= (mfac*cp1[1] + fac*((cp2[1]*cp1[1])/255))/255;
	cp[2]= (mfac*cp1[2] + fac*((cp2[2]*cp1[2])/255))/255;
	cp[3]= (mfac*cp1[3] + fac*((cp2[3]*cp1[3])/255))/255;
	return col;
}

unsigned int mcol_lighten(unsigned int col1, unsigned int col2, int fac)
{
	char *cp1, *cp2, *cp;
	int mfac;
	unsigned int col=0;

	if(fac==0) return col1;
	if(fac>=255) return col2;

	mfac= 255-fac;
	cp1= (char *)&col1;
	cp2= (char *)&col2;
	cp=  (char *)&col;
	if (cp1[1]+cp1[2]+cp1[3] > cp2[1]+cp2[2]+cp2[3])
		return col1;

	cp[0]= 255;
	cp[1]= (mfac*cp1[1]+fac*cp2[1])/255;
	cp[2]= (mfac*cp1[2]+fac*cp2[2])/255;
	cp[3]= (mfac*cp1[3]+fac*cp2[3])/255;
	return col;
}

unsigned int mcol_darken(unsigned int col1, unsigned int col2, int fac)
{
	char *cp1, *cp2, *cp;
	int mfac;
	unsigned int col=0;

	if(fac==0) return col1;
	if(fac>=255) return col2;

	mfac= 255-fac;
	cp1= (char *)&col1;
	cp2= (char *)&col2;
	cp=  (char *)&col;
	if (cp1[1]+cp1[2]+cp1[3] < cp2[1]+cp2[2]+cp2[3])
		return col1;

	cp[0]= 255;
	cp[1]= (mfac*cp1[1]+fac*cp2[1])/255;
	cp[2]= (mfac*cp1[2]+fac*cp2[2])/255;
	cp[3]= (mfac*cp1[3]+fac*cp2[3])/255;
	return col;
}
