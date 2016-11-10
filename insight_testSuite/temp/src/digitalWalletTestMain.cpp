// digitalWalletTestMain.cpp : Defines the entry point for the console application.
//

#include "DigitalWallet.h"
#include <iostream>
#include <string>
#include <ctime>

int main()
{
	DigitalWallet* payMoWallet = new DigitalWallet();
	std::cout << "Enter the path to a file containing authorized transactions from history: " << std::endl;
	std::string inputFileName;
	std::cin >> inputFileName;
	payMoWallet->createUserDatabaseFromPastDataFastIO(inputFileName);
	std::cout << "Enter the path and name of the output file for the verification results: " << std::endl;
	std::string outputFileName;
	std::cin >> outputFileName;
	int alertLevel = -1;
	while (! (alertLevel == 1 || alertLevel==2 || alertLevel==4) ) {
		std::cout << "Enter intended alert Level (1 for direct contact, 2 for 2nd degree contact, 4 for 4th degree contact, others not allowed): " << std::endl;
		std::cin >> alertLevel;
	}
	long long suffix = (alertLevel==4?3:alertLevel);
	std::string outputPath = "./paymo_output/output" + std::to_string((long long)suffix) + ".txt";
	std::clock_t beforeClock = std::clock();
	payMoWallet->verifyPaymentTrans(outputFileName, outputPath, alertLevel);
	std::clock_t afterClock = std::clock();
	std::cout << "analysis done in " << (afterClock - beforeClock) << " ms" << std::endl;
	return 0;
}

