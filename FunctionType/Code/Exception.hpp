#ifndef __EXCEPTION_HPP__
#define __EXCEPTION_HPP__
#include <stdexcept>

namespace WinSvcLib {
	std::runtime_error GetLastException();
}
#endif
