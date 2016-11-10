#include "TransLog.h"



void TransLog::fillFieldsFastIO(const char *lineBuffer) {

		// parse date & time
		//std::tm* timeStruct = new std::tm();
		// removed all legalty check in order to boost up the performance
		sscanf(lineBuffer, "%d-%d-%d %d:%d:%d, %d, %d, %lf, %s", &timeStruct->tm_year, &timeStruct->tm_mon, &timeStruct->tm_mday,
																  &timeStruct->tm_hour, &timeStruct->tm_min, &timeStruct->tm_sec,
																  &payerID, &receiverID,
																  &amount, messageBuf);
		timeStruct->tm_year -= 1900;			// by convention, the year expression is the offset from 1900
		timeStruct->tm_mon -= 1;				//	the month expression is also zero based
		
		transTime = std::mktime(timeStruct);
		messageStr.assign(this->messageBuf);
		//delete timeStruct;
}

TransLog::TransLog(void) {
	this->timeStruct = new std::tm();
	this->messageBuf = new char[1000];
}

TransLog::~TransLog(void) {
	delete this->timeStruct;
	delete[] this->messageBuf;
	messageBuf = NULL;
}


