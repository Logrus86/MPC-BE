/*
 * (C) 2026 see Authors.txt
 *
 * This file is part of MPC-BE.
 *
 * MPC-BE is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 3 of the License, or
 * (at your option) any later version.
 *
 * MPC-BE is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 *
 */

#pragma once

#include <atlstr.h>
#include "AppSettings.h" // dispmode

// Discovery helper for HDMI 1.4a Frame Packing (1920x2205 with 45-line gap).
// The mode must already exist in the OS (e.g. created via CRU).
namespace FramePacking3DFullscreen {

	// Find a 1920x2205 mode (32 bpp, progressive). If `preferredHz` > 0,
	// look for that rate first; otherwise probe 120/60/24 Hz in priority
	// order. Returns the matching dispmode and the resolved GDI device name.
	bool FindFpDispMode(const CStringW& displayName,
	                    int preferredHz,
	                    dispmode& outDm,
	                    CStringW& outDisplayName);
}
