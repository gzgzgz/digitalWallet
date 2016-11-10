#pragma once
#include <ctime>
#include <cwchar>
#include <string>
#include <exception>
#include <iostream>
#include <sstream>
#include <vector>

struct TransLog {
	std::time_t transTime;
	int payerID;
	int receiverID;
	double amount;
	std::string messageStr;

	// use as buffer for time formatting
	std::tm* timeStruct;
	char *messageBuf;

	TransLog(void);		// pay attention to wstring case in here
	void fillFieldsFastIO(const char *lineBuffer);
	~TransLog(void);
};

