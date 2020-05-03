#pragma once
#include <dwmapi.h>

static unsigned int monitorSearchIndex = 0;
static HDC monitorDC = 0;

/*
 * FUNCTION MonitorEnumProc
 *
 * this is a Win32 callback function which is given as an argument
 * to EnumDisplayMonitors.
 *
 * returns
 * TRUE: if the current enumerated display is the wrong one
 * FALSE: if the right monitor was found and the DC was associated
 */
BOOL CALLBACK MonitorEnumProc(HMONITOR monitor, HDC hdc, LPRECT rect, LPARAM data)
{
    MONITORINFOEX monitorInfo;

    if (monitorSearchIndex++ != (unsigned int)data)
        return TRUE; /* continue enumeration */

    monitorInfo.cbSize = sizeof(monitorInfo);
    if (GetMonitorInfo(monitor, (LPMONITORINFO)&monitorInfo))
        monitorDC = CreateDC(NULL, monitorInfo.szDevice, NULL, NULL);

    return FALSE;  /* stop enumeration */
}

/*
 * FUNCTION FindMonitor
 *
 * find a specific monitor given by index. Index -1 is the
 * primary display.
 *
 * returns the DC of the selected monitor
 */
HDC FindMonitor(int index)
{
    if (index == -1)
        return GetDC(NULL); /* return primary display context */

    monitorSearchIndex = 0;
    monitorDC = 0;
    EnumDisplayMonitors(NULL, NULL, MonitorEnumProc, index);
    return monitorDC;
}