#include "stdafx.h"
#include "ServiceCreate.hpp"
#include <strsafe.h>
#include <process.h>
#include <string>
#include <vector>
static bool UseWinSvcLibStandardHandlerEx;

namespace WinSvcLib {
	void CommonUpdateNewState(WinSvcInfo &Info, const ServiceState NewState) {
		Info.SvcStatus.dwCurrentState = static_cast<DWORD>(NewState);
		Info.CurrentServiceState = NewState;
	}
	WinSvcInfo CommonWinSvcLibInit(LPCTSTR ServiceName, HANDLERFUNCTION HandlerEx, const ServiceType WindowsServiceType) {
		WinSvcInfo Svc;
		Svc.SvcStatusHandle = RegisterServiceCtrlHandlerEx(ServiceName, HandlerEx, NULL);
		memset(&Svc.SvcStatus, 0, sizeof(Svc.SvcStatus));
		Svc.SvcStatus.dwServiceType = static_cast<DWORD>(WindowsServiceType);
		Svc.SvcStatus.dwCurrentState = static_cast<DWORD>(ServiceState::StartPending);
		Svc.SvcStatus.dwControlsAccepted = static_cast<DWORD>(ServiceControlsAccepted::Stop | ServiceControlsAccepted::PauseContinue);
		Svc.SvcStatus.dwWin32ExitCode = NO_ERROR;
		Svc.SvcStatus.dwServiceSpecificExitCode = 0;
		Svc.SvcStatus.dwCheckPoint = 0;
		Svc.SvcStatus.dwWaitHint = 2000;
		Svc.CurrentServiceState = ServiceState::StartPending;
		return Svc;
	}

	int CommonRun(WinSvcInfo &Info, SERVICETHREAD ServiceThread, LPVOID ArgList) {
		UINT uiThreadID;
		_beginthreadex(NULL, 0, ServiceThread, ArgList, 0, &uiThreadID);
		return SetServiceStatus(Info.SvcStatusHandle, &Info.SvcStatus);
	}

	namespace Original {
		WinSvcInfo WinSvcLibInit(LPCTSTR ServiceName, HANDLERFUNCTION HandlerEx, const ServiceType WindowsServiceType) {
			UseWinSvcLibStandardHandlerEx = false;
			return CommonWinSvcLibInit(ServiceName, HandlerEx, WindowsServiceType);
		}
		WinSvcInfo WinSvcLibInit(const std::basic_string<TCHAR> ServiceName, HANDLERFUNCTION HandlerEx, const ServiceType WindowsServiceType) {
			UseWinSvcLibStandardHandlerEx = false;
			return CommonWinSvcLibInit(ServiceName.c_str(), HandlerEx, WindowsServiceType);
		}
		void SetServiceType(WinSvcInfo& Info, const ServiceType SvcType) {
			Info.SvcStatus.dwServiceType = static_cast<DWORD>(SvcType);
		}
		void SetCurrentState(WinSvcInfo& Info, const ServiceState SvcState) {
			Info.SvcStatus.dwCurrentState = static_cast<DWORD>(SvcState);
		}
		void SetControlsAccepted(WinSvcInfo& Info, const ServiceControlsAccepted SvcControlAccepted) {
			Info.SvcStatus.dwControlsAccepted = static_cast<DWORD>(SvcControlAccepted);
		}
		void SetWin32ExitCode(WinSvcInfo& Info, const DWORD Win32ExitCode) {
			Info.SvcStatus.dwWin32ExitCode = Win32ExitCode;
		}
		void SetServiceSpecificExitCode(WinSvcInfo& Info, const DWORD ServiceSpecificExitCode) {
			Info.SvcStatus.dwServiceSpecificExitCode = ServiceSpecificExitCode;
		}
		void SetCheckPoint(WinSvcInfo& Info, const DWORD CheckPoint) {
			Info.SvcStatus.dwCheckPoint = CheckPoint;
		}
		void SetWaitHint(WinSvcInfo& Info, const DWORD WaitHint) {
			Info.SvcStatus.dwWaitHint = WaitHint;
		}
		void SetServiceStatus(WinSvcInfo& Info) {
			SetServiceStatus(Info.SvcStatusHandle, &Info.SvcStatus);
		}
		int Run(WinSvcInfo &Info, SERVICETHREAD ServiceThread, LPVOID ArgList) {
			return CommonRun(Info, ServiceThread, ArgList);
		}
		void UpdateNewState(WinSvcInfo& Info, const ServiceState NewState) {
			CommonUpdateNewState(Info, NewState);
		}
	}
	namespace StandardWin32OwnProcess {
		WinSvcInfo Inf;
		DWORD WINAPI WinSvcStdHandlerEx(DWORD dwControl, DWORD dwEventType, LPVOID lpEventData, LPVOID lpContext) {
			UNREFERENCED_PARAMETER(dwEventType);
			UNREFERENCED_PARAMETER(lpEventData);
			UNREFERENCED_PARAMETER(lpContext);
			Inf.CurrentServiceControl = static_cast<ServiceControl>(dwControl);
			switch (dwControl) {
				case SERVICE_CONTROL_STOP:
					CommonUpdateNewState(Inf, ServiceState::StopPending);
					break;
				case SERVICE_CONTROL_PAUSE:
					CommonUpdateNewState(Inf, ServiceState::PausePending);
					break;
				case SERVICE_CONTROL_CONTINUE:
					CommonUpdateNewState(Inf, ServiceState::ContinuePending);
					break;
				case SERVICE_CONTROL_INTERROGATE:
					SetServiceStatus(Inf.SvcStatusHandle, &Inf.SvcStatus);
					break;
				case SERVICE_CONTROL_SHUTDOWN:
					CommonUpdateNewState(Inf, ServiceState::StopPending);
					break;
				case SERVICE_CONTROL_PRESHUTDOWN:
					CommonUpdateNewState(Inf, ServiceState::StartPending);
					break;
				default:
					break;
			}
			return NO_ERROR;
		}
		void WinSvcLibInit(LPCTSTR ServiceName) {
			UseWinSvcLibStandardHandlerEx = true;
			Inf = CommonWinSvcLibInit(ServiceName, WinSvcStdHandlerEx, ServiceType::Win32OwnProcess);
		}
		void WinSvcLibInit(const std::basic_string<TCHAR> ServiceName) {
			UseWinSvcLibStandardHandlerEx = true;
			Inf = CommonWinSvcLibInit(ServiceName.c_str(), WinSvcStdHandlerEx, ServiceType::Win32OwnProcess);
		}
		void SetServiceType(const ServiceType SvcType) {
			Inf.SvcStatus.dwServiceType = static_cast<DWORD>(SvcType);
		}
		void SetCurrentState(const ServiceState SvcState) {
			Inf.SvcStatus.dwCurrentState = static_cast<DWORD>(SvcState);
		}
		void SetControlsAccepted(const ServiceControlsAccepted SvcControlAccepted) {
			Inf.SvcStatus.dwControlsAccepted = static_cast<DWORD>(SvcControlAccepted);
		}
		void SetWin32ExitCode(const DWORD Win32ExitCode) {
			Inf.SvcStatus.dwWin32ExitCode = Win32ExitCode;
		}
		void SetServiceSpecificExitCode(const DWORD ServiceSpecificExitCode) {
			Inf.SvcStatus.dwServiceSpecificExitCode = ServiceSpecificExitCode;
		}
		void SetCheckPoint(const DWORD CheckPoint) {
			Inf.SvcStatus.dwCheckPoint = CheckPoint;
		}
		void SetWaitHint(const DWORD WaitHint) {
			Inf.SvcStatus.dwWaitHint = WaitHint;
		}
		int Run(SERVICETHREAD ServiceThread, LPVOID ArgList) {
			return CommonRun(Inf, ServiceThread, ArgList);
		}
		void UpdateNewState(const ServiceState NewState) {
			CommonUpdateNewState(Inf, NewState);
		}
	}

}

