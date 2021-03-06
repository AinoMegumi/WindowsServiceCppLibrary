#ifndef __KGWINEXCEPTION_HPP__
#define __KGWINEXCEPTION_HPP__
#include <stdexcept>
#include <string>
#include <iostream>

class KgWinException : public std::exception {
private:
	std::string GetErrorMessage();
	std::string GetErrorMessage(const unsigned long ErrorCode);
public:
	KgWinException() : std::exception(this->GetErrorMessage().c_str()) {}
	KgWinException(const unsigned long ErrorCode) : std::exception(this->GetErrorMessage(ErrorCode).c_str()) {}
	KgWinException(const std::string& ExceptionAddMessage, const unsigned long ErrorCode) : std::exception((ExceptionAddMessage + "\n" + this->GetErrorMessage(ErrorCode)).c_str()) {}
	KgWinException(const std::string ExceptionMessage) : std::exception(ExceptionMessage.c_str()) {}
	KgWinException(const char* ExceptionMessage) : std::exception(ExceptionMessage) {}
	KgWinException(std::string&& ExceptionMessage) : std::exception(std::move(ExceptionMessage).c_str()) {}
	int GraphErrorMessageOnMessageBox(const char* Caption, const int Button = 0) const;
	int GraphErrorMessageOnMessageBox(const std::string& Caption, const int Button = 0) const { return this->GraphErrorMessageOnMessageBox(Caption.c_str(), Button); }
	void GraphErrorMessageOnConsole() const;
};

#endif
