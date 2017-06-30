#ifndef __WINMAINFUNC_HPP__
#define __WINMAINFUNC_HPP__
#ifdef SERVICECREATE_EXPORTS
#define SVCCREATE __declspec(dllexport)
#else
#ifdef WINSVCLIB_SHARED_BUILD
#define SVCCREATE __declspec(dllimport)
#else
#define SVCCREATE
#endif
#endif
#include <Windows.h>
#include <stdexcept>

typedef void (WINAPI* SERVICEMAIN)(DWORD, LPTSTR[]);

namespace WinSvcLib {
	SVCCREATE void AddServiceInfo(const LPCTSTR ServiceName, SERVICEMAIN ServiceMain);
	SVCCREATE void Dispatcher();
}
#endif
