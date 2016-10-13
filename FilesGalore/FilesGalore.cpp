#include "stdafx.h"
#include <fstream>
#include <ostream>
#include <ctime>
#include <string>
#include <iostream>
#include <sstream>
#include <Windows.h>

static const char alphanum[] =
"0123456789"
"!@#$%^&*"
"ABCDEFGHIJKLMNOPQRSTUVWXYZ"
"abcdefghijklmnopqrstuvwxyz";

const int _MAXSTRING = 1024;

//int stringLength = sizeof(alphanum) - 1;
int leftToWrite;
int numberOfFiles = 0;
int fileBytes = 0; //i.e. characters in the file

void sortTheArgs(int numArgs, char *args[])
{
	std::string arg1;
	std::string arg2;

	for (int i = 0; i < numArgs; i++)
	{
		std::string s = args[i];
		if (s.find("-f") != std::string::npos)
		{
			arg1 = s.erase(0, 2);
			std::stringstream ss(arg1);
			ss >> numberOfFiles;
		}
		if (s.find("-b") != std::string::npos)
		{
			arg2 = s.erase(0, 2);
			std::stringstream ss(arg2);
			ss >> fileBytes;
		}
	}
}

void writeJunkData(std::string file, int characters)
{
	char buffer[_MAXSTRING];

	for (unsigned int x = 0; x < characters; x++)
	{
		buffer[x] = alphanum[rand() % characters];
	}

	std::ofstream mStream(file, std::ios::in | std::ios::out | std::ios::binary | std::ios::app);

	mStream.write(buffer, characters);
	mStream.clear();

}

void createJunk()
{
	CreateDirectoryA(".\\JunkDirectory", NULL);

	for (long i = 0; i < numberOfFiles; i++)
	{
		printf_s("doing %d\n", i + 1);

		srand(i);
		std::stringstream filenameStream;

		filenameStream << ".\\JunkDirectory\\junkfile" << i << ".bin";

		std::string filename = filenameStream.str();

		leftToWrite = fileBytes;

		bool Done = false;

		while (!Done)
		{
			if (leftToWrite > _MAXSTRING)
			{
				writeJunkData(filename, 1024);
				leftToWrite = leftToWrite - 1024;
			}
			else
			{
				writeJunkData(filename, leftToWrite);
				Done = true;
			}
		}

		filenameStream.clear();

	}
}



int main(int argc, char *argv[])
{
	if (argc > 3)
	{
		std::cout << "Incorrect number of arguments.\nPlease use -f#### for number of files, -b#### for byte size desired.\n";
		system("PAUSE");
		return 1;
	}

	std::string input;

	if (argc > 1)
	{
		sortTheArgs(argc, argv);
	}

	//defaults if no args
	if (numberOfFiles == 0) { numberOfFiles = 1000000; }
	if (fileBytes == 0) { fileBytes = 40; }

	char warningBuff[100];
	ZeroMemory(&warningBuff, sizeof(&warningBuff));
	snprintf(warningBuff, sizeof(warningBuff),
		"Are you sure you want to make %d %db junk files? (y/n)\n",
		numberOfFiles,
		fileBytes);

	std::cout << warningBuff;

	std::getline(std::cin, input);

	if (input == "y" || input == "Y")
	{

		createJunk();
	}
	else
	{
		std::cout << "Something went terribly, terribly wrong...\n";
		system("PAUSE");
		return 1;
	}
	std::cout << "All done!\n";
	system("PAUSE");
	return 0;
}


