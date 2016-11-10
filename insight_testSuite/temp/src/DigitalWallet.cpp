#include "DigitalWallet.h"


DigitalWallet::DigitalWallet(void) {
		MAX_USER_NO = 1<<24;
		mapUserIDtoIndex = new int[MAX_USER_NO];
		for(int i=0; i<MAX_USER_NO; ++i)
			mapUserIDtoIndex[i]=-1;
}

DigitalWallet::DigitalWallet(int maxUserNoInput) {
		MAX_USER_NO = maxUserNoInput;
		mapUserIDtoIndex = new int[MAX_USER_NO];
		for(int i=0; i<MAX_USER_NO; ++i)
			mapUserIDtoIndex[i]=-1;
}

int DigitalWallet::getMaxUserNo() {
	return MAX_USER_NO; 
}


DigitalWallet::~DigitalWallet(void) {
		delete[] mapUserIDtoIndex;
}


int DigitalWallet::checkUserRelationShipByBFS(int payerIndex, int receiverIndex, int alertLevel) {
	std::queue<int> bufferQueue;
	bufferQueue.push(payerIndex);
	visitedCheckTable.insert(payerIndex);
	for(int i=0; i<alertLevel; ++i) {
		int curQueueSize = bufferQueue.size();
		for(int j=0; j<curQueueSize; ++j) {
			int curEle = bufferQueue.front();
			if (friendCircleGraphHashingFast[curEle].find(receiverIndex) != friendCircleGraphHashingFast[curEle].end()) return i+1;
			bufferQueue.pop();
			for(auto iter = friendCircleGraphHashingFast[curEle].begin(); iter!=friendCircleGraphHashingFast[curEle].end(); ++iter) {
				if (visitedCheckTable.find(*iter)==visitedCheckTable.end()) {
					bufferQueue.push(*iter);
					visitedCheckTable.insert(*iter);
				}
			}
		}
	}
	return INT_MAX;
}
int DigitalWallet::checkUserRelationShipByDFS(int payerIndex, int receiverIndex, int curDFSLevel, int alertLevel) {
	// recursion version
	if (curDFSLevel>alertLevel) return INT_MAX;	// fail to find

	if ( friendCircleGraphHashingFast[payerIndex].find(receiverIndex) == friendCircleGraphHashingFast[payerIndex].end() ) {
		// not found in current level, proceed to the next level
		for(auto iter = friendCircleGraphHashingFast[payerIndex].begin(); iter!=friendCircleGraphHashingFast[payerIndex].end(); ++iter) {
			if ( visitedCheckTable.find((*iter)) == visitedCheckTable.end() ) {
				visitedCheckTable.insert(*iter);
				int status = checkUserRelationShipByDFS( (*iter), receiverIndex, curDFSLevel+1, alertLevel);
				if (status<=alertLevel) return status;	
				visitedCheckTable.erase(*iter);
			}
		}
		return INT_MAX;
	} else return curDFSLevel;

}

inline void DigitalWallet::addIncomingTransFast(TransLog const &curTrans) {
	// first check if current Transaction involves any new user (either payer or receiver)
	bool PayerFound = (mapUserIDtoIndex[curTrans.payerID] != -1);
	bool ReceiverFound = (mapUserIDtoIndex[curTrans.receiverID] != -1);
	if ( PayerFound && ReceiverFound ) {
		friendCircleGraphHashingFast[mapUserIDtoIndex[curTrans.payerID]].insert(mapUserIDtoIndex[curTrans.receiverID]);
		friendCircleGraphHashingFast[mapUserIDtoIndex[curTrans.receiverID]].insert(mapUserIDtoIndex[curTrans.payerID]);
		return;
	}
	int curSize = userDatabaseFast.size();
	if ( !PayerFound ) {
		// payer not in the database
		std::unordered_set<int> friendCircle;
		userDatabaseFast.push_back(curTrans.payerID);
		mapUserIDtoIndex[curTrans.payerID] = curSize++;
		friendCircleGraphHashingFast.push_back(friendCircle);
	} 
		
	if ( !ReceiverFound ) {
		// receiver not exist either
		userDatabaseFast.push_back(curTrans.receiverID);
		mapUserIDtoIndex[curTrans.receiverID] = curSize;
		std::unordered_set<int> recvCir;
		recvCir.insert(mapUserIDtoIndex[curTrans.payerID]);
		friendCircleGraphHashingFast.push_back(recvCir);
	} 

	// establish the link
	friendCircleGraphHashingFast[mapUserIDtoIndex[curTrans.payerID]].insert(mapUserIDtoIndex[curTrans.receiverID]); 
}


int DigitalWallet::checkFraud(TransLog const &curTrans, int alertLevel) {
	int payerID = curTrans.payerID;
	int receiverID = curTrans.receiverID;
	int fraudCheckStatus;
	// check and make sure both payerID and receiverID both present in the current network
	// if the receiverID does not exist in our database, we directly give the alert no matter what
	bool payerFound = (mapUserIDtoIndex[payerID] != -1);
	bool receiverFound = (mapUserIDtoIndex[receiverID] != -1);
	if ( !receiverFound ) {
			fraudCheckStatus = INT_MAX;	
	} else {
		if ( !payerFound )	addIncomingTransFast(curTrans);
		int payerIndex = mapUserIDtoIndex[payerID];
		int receiverIndex = mapUserIDtoIndex[receiverID];


		switch (alertLevel) {
			case 1:
				if (friendCircleGraphHashingFast[payerIndex].find(receiverIndex) != friendCircleGraphHashingFast[payerIndex].end()) return 0;
				else return 1;
			case 2: {
				if (friendCircleGraphHashingFast[payerIndex].find(receiverIndex) != friendCircleGraphHashingFast[payerIndex].end()) return 0;
				break;
			}
			case 4: {
				if (friendCircleGraphHashingFast[payerIndex].find(receiverIndex) != friendCircleGraphHashingFast[payerIndex].end()) return 0;
				break;
			}
			default:
				std::cerr << "Illegal alert level defined! must be 1, 2, and 4 in current setting" << '\n';
				exit(1);
		}

		visitedCheckTable.clear();
//		 fraudCheckStatus = checkUserRelationShipByDFS(payerIndex, receiverIndex, 1, alertLevel);	
		fraudCheckStatus = checkUserRelationShipByBFS(payerIndex, receiverIndex, alertLevel);
	}

	if (fraudCheckStatus == INT_MAX) {
		switch (alertLevel) {
		case 4:
			return 3;		// represent unverified status that is beyond 4th degree neighbor
		case 2:
			return 2;		// same as above, beyond 2nd degree neighbor
		case 1:
			return 1;		// same as above, beyond first neighbor 
		default:
			std::cerr << "Illegal alert level defined! must be 1, 2, and 4 in current setting" << '\n';
			exit(1);
		}
	} else	return 0;			// return value for trusted cases
	

}


void DigitalWallet::createUserDatabaseFromPastDataFastIO(std::string const &TransFileName) {
	try {
		std::FILE* inputFileStream;
		inputFileStream =fopen(TransFileName.c_str(), "r");
		char* lineBuffer = new char[1000];
		if(inputFileStream==NULL)
			throw std::runtime_error("open file error!");
		fgets(lineBuffer, 1000, inputFileStream);	// skip the header
		int counter = 0;
		TransLog curTrans;
		while(fgets(lineBuffer, 1000, inputFileStream) != NULL) {
			curTrans.fillFieldsFastIO(lineBuffer);
			addIncomingTransFast(curTrans);
			++counter;
			if (counter%100000 == 0) {
				std::cout << counter <<" records constructed..."<< std::endl;
 			}
		}
		fclose(inputFileStream);
		delete[] lineBuffer;
	} catch (std::runtime_error e) {
		std::cerr << "Error in module createUserDatabaseFromPastData: Exception in reading file I/O. " << e.what() << '\n';
	} 
}

void DigitalWallet::verifyPaymentTrans(std::string const &TransFileName, std::string const &checkFraudOutputFileName, int alertLevel) {
	try {
		std::FILE* inputFileStream;
		inputFileStream =fopen(TransFileName.c_str(), "r");
		char* lineBuffer = new char[1000];
		if(inputFileStream==NULL)
			throw std::runtime_error("open file error!");
		fgets(lineBuffer, 1000, inputFileStream);	// skip the header
		TransLog curTrans;

		std::ofstream outputVerFileStream;
		outputVerFileStream.open(checkFraudOutputFileName);
		int counter=0;
		while( fgets(lineBuffer, 1000, inputFileStream) != NULL ) {
			curTrans.fillFieldsFastIO(lineBuffer);
			int status = checkFraud(curTrans, alertLevel);
			if (status == 1 || status == 2 || status == 3)
				outputVerFileStream << "unverified" << std::endl;
			else
				outputVerFileStream << "trusted" << std::endl;
			++counter;
			if (counter%1000 == 0) {
				std::cout << counter << std::endl;
			}
		}
		fclose(inputFileStream);
		outputVerFileStream.close();
		delete[] lineBuffer;
	} catch (std::runtime_error e) {
		std::cerr << "Error in module verifyPaymentTrans: Exception in reading/writing file I/O of files"  << "\nErr code:" << e.what() << '\n';
	} 
}
