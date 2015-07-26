#include "SmartFile.h"

#include <iostream>

using namespace std;

void OpenWithoutName()
{
	SmartFile file;

	try
	{
		file.open("");
	}
	catch (exception& ex)
	{
		cout << "Exception: " << ex.what() << endl;
	}
}

void WorkWithFile()
{
	string sFileName = "test.txt";

	//open file for write some data
	SmartFile file;
	file.open(sFileName, ios::out);
	if (!file.is_open())
	{
		cout << "File not opened!!!";
		return;
	}

	file << "Hello, world!"; //put data to file

	file.reopen(ios::in); //reopen file in read mode
	
	//read all data from file
	char symbol;
	while (file.read(&symbol, 1))
		cout << symbol;
	cout << endl;

	
	file.reopen(ios::in | ios::out | ios::app); //reopen file and append some data
	file << "\nNew Line";

	//read all data from file
	file.seekToOffset(0);
	cout << "---------------" << endl;
	while (file.read(&symbol, 1))
		cout << symbol;
	cout << endl;
}

int main()
{
	//OpenWithoutName();
	WorkWithFile();

	getchar();
	
}