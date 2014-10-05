#include "Input.h"

Parameters getpara()    // gets simulation parameters from inp001.txt
{
	double nu, T, E_M_M, E_M_Si, E_M_Tl, E_bM, E_b100, E_b111;


	int initH;
	double lps, targetCov, heatingRate;
	int seed;
	char fileName[] = "inp001.txt";
	std::ifstream inputFile;
	
	inputFile.open(fileName, std::ios::in);
	if (!inputFile.is_open())
		std::cout << "chyba pri otevirani inp";
	
	
	inputFile.ignore(1000, '='); inputFile >> nu;
	inputFile.ignore(1000, '='); inputFile >> T;
	
   inputFile.ignore(1000, '='); inputFile >> E_M_M;
   inputFile.ignore(1000, '='); inputFile >> E_M_Si;
	inputFile.ignore(1000, '='); inputFile >> E_M_Tl;
	inputFile.ignore(1000, '='); inputFile >> E_bM;
	inputFile.ignore(1000, '='); inputFile >> E_b100;
	inputFile.ignore(1000, '='); inputFile >> E_b111;

	inputFile.ignore(1000, '='); inputFile >> lps;
	inputFile.ignore(1000, '='); inputFile >> targetCov;

	inputFile.ignore(1000, '='); inputFile >> seed;
	
	inputFile.close();
	
	return Parameters(nu, T, E_M_M, E_M_Si, E_M_Tl, E_bM, E_b100, E_b111, lps, targetCov, seed);
}

Parameters inputPara = getpara();