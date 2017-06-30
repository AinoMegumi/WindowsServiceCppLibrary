#include "WinMainFunc.hpp"
#include "ExceptionManager.hpp"
#include "KgWinException.hpp"
#include <vector>
#include <type_traits>

std::vector<SERVICE_TABLE_ENTRY> ServiceData;

namespace WinSvcLib {
	void AddServiceInfo(const LPCTSTR ServiceName, SERVICEMAIN ServiceMain) {
		TCHAR Buffer[1024];
#ifdef UNICODE
		wcscpy_s(Buffer, ServiceName);
#else
		strcpy_s(Buffer, ServiceName);
#endif
		SERVICE_TABLE_ENTRY Ent = { Buffer, ServiceMain };
		ServiceData.emplace_back(Ent);
	}

	void Dispatcher() {
		try {
			SERVICE_TABLE_ENTRY* Table = new SERVICE_TABLE_ENTRY[ServiceData.size() + 1];
			for (size_t i = 0; i < ServiceData.size(); i++) Table[i] = ServiceData[i];
			Table[ServiceData.size()] = { NULL, NULL };
			if (0 == StartServiceCtrlDispatcher(Table)) throw KgWinException();
		}
		catch (const std::exception& er) {

		}
	}
}
