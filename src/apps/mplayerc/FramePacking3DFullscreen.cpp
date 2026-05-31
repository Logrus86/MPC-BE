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

#include "stdafx.h"
#include "FramePacking3DFullscreen.h"

namespace FramePacking3DFullscreen {

// HDMI 1.4a 1920x1080p Frame Packing: 1920 x (1080 + 45 + 1080) = 1920x2205.
static constexpr LONG kFpWidth  = 1920;
static constexpr LONG kFpHeight = 2205;

static CStringW ResolveDisplayName(const CStringW& displayName)
{
	if (!displayName.IsEmpty() && displayName != L"Current") {
		return displayName;
	}

	const HWND hWnd = AfxGetApp() && AfxGetApp()->m_pMainWnd
		? AfxGetApp()->m_pMainWnd->GetSafeHwnd()
		: nullptr;
	const HMONITOR hMon = MonitorFromWindow(hWnd ? hWnd : ::GetDesktopWindow(),
	                                         MONITOR_DEFAULTTOPRIMARY);
	MONITORINFOEXW mi = {};
	mi.cbSize = sizeof(mi);
	if (GetMonitorInfoW(hMon, &mi)) {
		return CStringW(mi.szDevice);
	}
	return CStringW();
}

// Same filter as PPageFullscreen uses for the FullscreenRes list.
static bool MatchOneRate(const CStringW& dev, DWORD wantHz, dispmode& outDm)
{
	DEVMODEW dm = {};
	dm.dmSize = sizeof(dm);
	for (DWORD i = 0; EnumDisplaySettingsW(dev, i, &dm); ++i) {
		if (dm.dmPelsWidth        == (DWORD)kFpWidth
		 && dm.dmPelsHeight       == (DWORD)kFpHeight
		 && dm.dmDisplayFrequency == wantHz
		 && dm.dmBitsPerPel       == 32
		 && !(dm.dmDisplayFlags & DM_INTERLACED)) {
			outDm.bValid          = true;
			outDm.size            = CSize(dm.dmPelsWidth, dm.dmPelsHeight);
			outDm.bpp             = (int)dm.dmBitsPerPel;
			outDm.freq            = (int)dm.dmDisplayFrequency;
			outDm.dmDisplayFlags  = dm.dmDisplayFlags;
			return true;
		}
	}
	return false;
}

bool FindFpDispMode(const CStringW& displayName,
                    int preferredHz,
                    dispmode& outDm,
                    CStringW& outDisplayName)
{
	const CStringW dev = ResolveDisplayName(displayName);
	outDisplayName = dev;
	if (dev.IsEmpty()) {
		return false;
	}

	// Try the cached rate first.
	if (preferredHz > 0 && MatchOneRate(dev, (DWORD)preferredHz, outDm)) {
		return true;
	}

	// Fallback: priority sweep.
	for (DWORD want : { 120u, 60u, 24u }) {
		if (MatchOneRate(dev, want, outDm)) {
			return true;
		}
	}
	return false;
}

} // namespace FramePacking3DFullscreen
