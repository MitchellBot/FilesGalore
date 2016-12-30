#include "stdafx.h"
#include <fstream>
#include <ostream>
#include <ctime>
#include <string>
#include <iostream>
#include <sstream>
#include <Windows.h>
#include <thread>
#include <mutex>

static const char alphanum[] =
"0123456789"
"!@#$%^&*"
"ABCDEFGHIJKLMNOPQRSTUVWXYZ"
"abcdefghijklmnopqrstuvwxyz";

const int _MAXSTRING = 1024;

std::mutex mMutex;

//int stringLength = sizeof(alphanum) - 1;
int leftToWrite;
int numberOfFiles = 0;
int fileBytes = 0; //i.e. characters in the file

bool isThreadOneDone = false;

void sortTheArgs(int numArgs, char *args[])
{
	std::string arg1;
	std::string arg2;

	for (int i = 0; i < numArgs; i++)
	{
		std::string s = args[i];
		if (s.find("-f") != std::string::npos)
		{
			std::stringstream ss(args[i + 1]);
			ss >> numberOfFiles;
		}
		if (s.find("-b") != std::string::npos)
		{
			std::stringstream ss(args[i + 1]);
			ss >> fileBytes;
		}
	}
}

void writeJunkData(std::string file, int characters)
{
	char buffer[_MAXSTRING];

	for (unsigned int x = 0; x < characters; x++)
	{
		buffer[x] = alphanum[rand() % sizeof(alphanum)];
	}

	std::ofstream mStream(file, std::ios::in | std::ios::out | std::ios::binary | std::ios::app);

	mStream.write(buffer, characters);
	mStream.clear();

}

void createJunk(int fileNumber)
{
	CreateDirectoryA(".\\JunkDirectory", NULL);

	std::stringstream filenameStream;

	filenameStream << ".\\JunkDirectory\\junkfile" << fileNumber + 1 << ".bin";

	std::string filename = filenameStream.str();

	mMutex.lock();
	leftToWrite = fileBytes;

	bool Done = false;

	while (!Done)
	{
		if (leftToWrite > _MAXSTRING)
		{
			writeJunkData(filename, 1024);
			leftToWrite -= 1024;
		}
		else
		{
			writeJunkData(filename, leftToWrite);
			Done = true;
		}
	}

	filenameStream.clear();
	mMutex.unlock();
}

void handleOddFiles()
{
	for (long i = 0; i < numberOfFiles; i++)
	{
		if (i % 2 == 0)
		{
			printf_s("doing %d\n", i + 1);
			createJunk(i);
		}
	}
}

void handleEvenFiles()
{
	for (long i = 0; i < numberOfFiles; i++)
	{
		if (i % 2 != 0)
		{
			printf_s("doing %d\n", i + 1);
			createJunk(i);
		}
	}
	isThreadOneDone = true;
}


int main(int argc, char *argv[])
{
	if (argc > 5)
	{
		std::cout << "Incorrect number of arguments.\nPlease use -f #### for number of files, -b #### for byte size desired.\n";
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
		std::thread t1(handleEvenFiles);
		std::thread t2(handleOddFiles);

		t1.detach();
		t2.join();
	}
	else
	{
		std::cout << "Why would you launch me if you weren't going to use me?\n";
		system("PAUSE");
		return 1;
	}

	while (!isThreadOneDone)
	{
		std::this_thread::yield();
	}

	std::cout << "All done!\n";
	system("PAUSE");
	return 0;
}


