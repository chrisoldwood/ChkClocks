/*****************************************************************************
** (C) Chris Wood 1995.
**
** DEBUGOUT.C - MDALIB32.DLL Thunking functions.
**
******************************************************************************
*/

#include <windows.h>

// Pointer to real OutputDebugString().
void (*DebugOut)(LPCSTR lpszMsg);

// DLL Instance handle.
extern HINSTANCE hDLLInstance;

// 16-bit call prototype
void PASCAL OutputMsg16(LPCSTR lpszMsg);

/*****************************************************************************
** This gets the address of the real OutputDebugString() function so that we
** can pass messages along.
*/
void GetRealDebugOutFn(void)
{
     (FARPROC)DebugOut = GetProcAddress(GetModuleHandle("KERNEL32"), "OutputDebugStringA");
}

/*****************************************************************************
** This redirects the OutputDebugString() call into our 16-bit DLL, to display
** the string on an MDA. It then passes it onto the kernel.
*/
void WINAPI OutputDebugString(LPCSTR lpszMsg)
{
     // Pass message onto 16bit DLL.
     OutputMsg16(lpszMsg);

     // Got real address?
     if (DebugOut)
          DebugOut(lpszMsg);
}
