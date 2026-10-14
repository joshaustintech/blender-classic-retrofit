/**
 * $Id: GHOST_DisplayManagerCocoa.mm 26841 2010-02-12 13:34:04Z campbellbarton $
 * ***** BEGIN GPL LICENSE BLOCK *****
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; either version 2
 * of the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software Foundation,
 * Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301, USA.
 *
 * The Original Code is Copyright (C) 2001-2002 by NaN Holding BV.
 * All rights reserved.
 *
 * The Original Code is: all of this file.
 *
 * Contributor(s):	Maarten Gribnau	09/2001
					Damien Plisson	10/2009
 *
 * ***** END GPL LICENSE BLOCK *****
 */

#include <Cocoa/Cocoa.h>

#include "GHOST_DisplayManagerCocoa.h"
#include "GHOST_Debug.h"

static CGDirectDisplayID displayID(GHOST_TUns8 display)
{
	NSArray *screens = [NSScreen screens];
	if (display >= [screens count]) return kCGNullDirectDisplay;
	return [[[[screens objectAtIndex:display] deviceDescription] objectForKey:@"NSScreenNumber"] unsignedIntValue];
}

static void displaySetting(CGDisplayModeRef mode, GHOST_TUns8 display, GHOST_DisplaySetting& setting)
{
	setting.xPixels = CGDisplayModeGetWidth(mode);
	setting.yPixels = CGDisplayModeGetHeight(mode);
	setting.bpp = NSBitsPerPixelFromDepth([[[NSScreen screens] objectAtIndex:display] depth]);
	setting.frequency = CGDisplayModeGetRefreshRate(mode);
}

GHOST_DisplayManagerCocoa::GHOST_DisplayManagerCocoa(void) {}

GHOST_TSuccess GHOST_DisplayManagerCocoa::getNumDisplays(GHOST_TUns8& count) const
{
	@autoreleasepool { count = [[NSScreen screens] count]; }
	return GHOST_kSuccess;
}

GHOST_TSuccess GHOST_DisplayManagerCocoa::getNumDisplaySettings(GHOST_TUns8 display, GHOST_TInt32& count) const
{
	@autoreleasepool {
		CGDirectDisplayID id = displayID(display);
		if (id == kCGNullDirectDisplay) return GHOST_kFailure;
		CFArrayRef modes = CGDisplayCopyAllDisplayModes(id, NULL);
		if (!modes) return GHOST_kFailure;
		count = CFArrayGetCount(modes);
		CFRelease(modes);
		return GHOST_kSuccess;
	}
}

GHOST_TSuccess GHOST_DisplayManagerCocoa::getDisplaySetting(GHOST_TUns8 display, GHOST_TInt32 index, GHOST_DisplaySetting& setting) const
{
	@autoreleasepool {
		CGDirectDisplayID id = displayID(display);
		if (id == kCGNullDirectDisplay) return GHOST_kFailure;
		CFArrayRef modes = CGDisplayCopyAllDisplayModes(id, NULL);
		if (!modes) return GHOST_kFailure;
		if (index < 0 || index >= CFArrayGetCount(modes)) { CFRelease(modes); return GHOST_kFailure; }
		displaySetting((CGDisplayModeRef)CFArrayGetValueAtIndex(modes, index), display, setting);
		CFRelease(modes);
		return GHOST_kSuccess;
	}
}

GHOST_TSuccess GHOST_DisplayManagerCocoa::getCurrentDisplaySetting(GHOST_TUns8 display, GHOST_DisplaySetting& setting) const
{
	@autoreleasepool {
		CGDirectDisplayID id = displayID(display);
		if (id == kCGNullDirectDisplay) return GHOST_kFailure;
		CGDisplayModeRef mode = CGDisplayCopyDisplayMode(id);
		if (!mode) return GHOST_kFailure;
		displaySetting(mode, display, setting);
		CGDisplayModeRelease(mode);
		return GHOST_kSuccess;
	}
}

GHOST_TSuccess GHOST_DisplayManagerCocoa::setCurrentDisplaySetting(GHOST_TUns8 display, const GHOST_DisplaySetting& requested)
{
	@autoreleasepool {
		GHOST_DisplaySetting match;
		if (findMatch(display, requested, match) != GHOST_kSuccess) return GHOST_kFailure;
		CGDirectDisplayID id = displayID(display);
		if (id == kCGNullDirectDisplay) return GHOST_kFailure;
		CFArrayRef modes = CGDisplayCopyAllDisplayModes(id, NULL);
		if (!modes) return GHOST_kFailure;
		GHOST_TSuccess result = GHOST_kFailure;
		for (CFIndex i = 0; i < CFArrayGetCount(modes); ++i) {
			CGDisplayModeRef mode = (CGDisplayModeRef)CFArrayGetValueAtIndex(modes, i);
			GHOST_DisplaySetting candidate;
			displaySetting(mode, display, candidate);
			if (candidate.xPixels == match.xPixels && candidate.yPixels == match.yPixels && candidate.frequency == match.frequency) {
				result = CGDisplaySetDisplayMode(id, mode, NULL) == kCGErrorSuccess ? GHOST_kSuccess : GHOST_kFailure;
				break;
			}
		}
		CFRelease(modes);
		return result;
	}
}
