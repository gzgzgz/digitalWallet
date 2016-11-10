#pragma once
#include <unordered_map>
#include <unordered_set>
#include <vector>
#include <climits>
#include <cstdio>
#include <cstdlib>
#include <queue>
#include <list>
#include <string>
#include <fstream>
#include <iostream>
#include <stdexcept>
#include <cwchar>
#include "TransLog.h"

class DigitalWallet {
private:
	// data fields
	int MAX_USER_NO;
	std::vector<int> userDatabaseFast;
	std::vector<std::unordered_set<int>> friendCircleGraphHashingFast;
	int* mapUserIDtoIndex;
	std::unordered_set<int> visitedCheckTable;


	// One method for checking all alertLevel of 1, 2, and 4 
	int checkUserRelationShipByDFS(int payerIndex, int receiverIndex, int curDFSlevel, int alertLevel);	
	int checkUserRelationShipByBFS(int payerIndex, int receiverIndex, int alertLevel);	// better performance
	inline void addIncomingTransFast(TransLog const &curTrans);

public:
	DigitalWallet(void);
	DigitalWallet(int maxUserNoInput);
	~DigitalWallet(void);
	int getMaxUserNo(void);
	int checkFraud(TransLog const &curTrans, int alertLevel);	
	void createUserDatabaseFromPastDataFastIO(std::string const &TransFileName);
	void verifyPaymentTrans(std::string const &TranFileName, std::string const &checkFraudOutputFileName, int alertLevel);
};

