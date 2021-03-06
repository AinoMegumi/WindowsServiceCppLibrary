#include "stdafx.h"
#include "Installer.hpp"
#include "KgWinException.hpp"
#include <shellapi.h>
#include <ShlObj.h>
#include <memory>

const std::basic_string<TCHAR> PowerShellPath =
#ifdef UNICODE
L"C:\\Windows\\System32\\WindowsPowerShell\\v1.0\\powershell.exe";
#else
"C:\\Windows\\System32\\WindowsPowerShell\\v1.0\\powershell.exe";
#endif

namespace WinSvcLib {
	namespace Installer {

		static inline String Import(const std::basic_string<TCHAR> Strings) {
#ifdef WINSVCLIB_SHARED_BUILD
			return std::make_shared<std::basic_string<TCHAR>>(Strings);
#else
			return Strings;
#endif
		}
		static inline std::basic_string<TCHAR> ChangeUserEnumToString(const ExecuteUser User) {
			switch (User) {
			case ExecuteUser::LocalService: return TEXT("NT AUTHORITY\\LocalService");
			case ExecuteUser::NetworkService: return TEXT("NT AUTHORITY\\NetworkService");
			default: return TEXT("NT AUTHORITY\\SYSTEM");
			}
		}

		void RunasAdministratorsAuthority() {
			if (FALSE == IsUserAnAdmin()) {
				TCHAR Buffer[MAX_PATH];
				if (0 == GetModuleFileName(NULL, Buffer, sizeof(Buffer))) throw KgWinException();
				std::basic_string<TCHAR> Path;
				Path.resize(MAX_PATH);
				Path = Buffer;
				Path.resize(std::char_traits<TCHAR>::length(Path.c_str()));
				ShellExecute(NULL, TEXT("open"), (PowerShellPath + TEXT(" Start-Process -Verb runas ") + Path).c_str(), NULL, NULL, SW_HIDE);
				exit(0);
			}
		}

		ServiceInstallInformation::ServiceInstallInformation() 
			: ServiceName(), DisplayName(), Description(), ExecuteFilePath(), Type(ServiceType::Win32OwnProcess),
			StartType(ServiceStartType::ManualStart), ErrorControl(ServiceErrorControlType::Ignore),
			LoadOrderGroup(), Dependencies(), ExecuteUser(Import(ChangeUserEnumToString(ExecuteUser::System))),
			ExecuteUserPassword(TEXT("")), DeviceDriver(false) {}


		ServiceInstallInformation InstallerLibInit(const std::basic_string<TCHAR> ServiceName, const std::basic_string<TCHAR> ExecuteFilePath, const bool IsDeviceDriver = false) {
			if (ServiceName.empty() || ExecuteFilePath.empty()) throw std::runtime_error("サービス名と実行ファイルのパスは必ず指定しなくてはなりません。");
			ServiceInstallInformation inf = ServiceInstallInformation();
			inf.ServiceName = ServiceName;
			inf.ExecuteFilePath = ExecuteFilePath;
			inf.DeviceDriver = IsDeviceDriver;
		}

		void ChangeServiceType(const ServiceType NewType, ServiceInstallInformation &Inf) {
			Inf.Type = NewType;
		}

		void ChangeStartType(const ServiceStartType NewType, ServiceInstallInformation &Inf) {
			Inf.StartType = NewType;
		}
		
		void ChangeDisplayName(const std::basic_string<TCHAR> DisplayName, ServiceInstallInformation &Inf) {
			Inf.DisplayName = Import(DisplayName);
		}
		
		void SetLoadOrderGroup(const std::basic_string<TCHAR> LoadOrderGroup, ServiceInstallInformation &Inf) {
			Inf.LoadOrderGroup = Import(LoadOrderGroup);
		}
		
		void SetDependencies(LPCTSTR Dependencies, ServiceInstallInformation &Inf) {
			Inf.Dependencies = Dependencies;
		}
		
		void ChangeExecuteUser(const std::basic_string<TCHAR> UserName, ServiceInstallInformation &Inf) {
			Inf.ExecuteUser = Import(UserName);
		}
		
		void ChangeExecuteUser(const ExecuteUser User, ServiceInstallInformation &Inf) {
			Inf.ExecuteUser = Import(ChangeUserEnumToString(User));
		}
		
		void SetUserPassword(const std::basic_string<TCHAR> Password, ServiceInstallInformation &Inf) {
			
		}
		
		void InstallService(const ServiceInstallInformation Inf, LPDWORD lpdwTagId) {

		}
	}
}
