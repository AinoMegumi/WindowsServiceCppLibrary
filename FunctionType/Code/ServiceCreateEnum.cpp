#include "stdafx.h"
#include "ServiceCreateEnum.hpp"
#include "EnumClassOperator.hpp"

namespace WinSvcLib {
	ServiceControl operator | (const ServiceControl A, const ServiceControl B) {
		return Operator(A, B);
	}

	ServiceControlsAccepted operator | (const ServiceControlsAccepted A, const ServiceControlsAccepted B) {
		return Operator(A, B);
	}
}
