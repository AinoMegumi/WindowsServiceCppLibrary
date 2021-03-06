#include "stdafx.h"
#include "WinMainFunc.hpp"
#include "ExceptionManager.hpp"
#include "KgWinException.hpp"
#include "StringCopy.hpp"
#include <vector>
#include <type_traits>

std::vector<SERVICE_TABLE_ENTRY> ServiceData;

namespace WinSvcLib {
	int WinSvcAddServiceInfo(const LPCTSTR ServiceName, SERVICEMAIN ServiceMain) {
		try {
			TCHAR Buffer[1024];
			tcscpy_s(Buffer, ServiceName);
			SERVICE_TABLE_ENTRY Ent = { Buffer, ServiceMain };
			ServiceData.emplace_back(Ent);
			return 0;
		}
		catch (const std::exception& er) {
			SetLastException(er.what());
			return -1;
		}
	}

	int AddServiceInfo(const LPCTSTR ServiceName, SERVICEMAIN ServiceMain) {
		return WinSvcAddServiceInfo(ServiceName, ServiceMain);
	}

	int AddServiceInfo(const std::basic_string<TCHAR> ServiceName, SERVICEMAIN ServiceMain) {
		return WinSvcAddServiceInfo(ServiceName.c_str(), ServiceMain);
	}

	int Dispatcher() {
		try {
			SERVICE_TABLE_ENTRY* Table = new SERVICE_TABLE_ENTRY[ServiceData.size() + 1];
			for (size_t i = 0; i < ServiceData.size(); i++) Table[i] = ServiceData[i];
			Table[ServiceData.size()] = { NULL, NULL };
			if (0 == StartServiceCtrlDispatcher(Table)) throw KgWinException();
			return 0;
		}
		catch (const std::exception& er) {
			SetLastException(er.what());
			return -1;
		}
	}
}
