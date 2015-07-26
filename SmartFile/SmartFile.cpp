/*
* SmartFile.h
*
*  Created on: 7.11.2014
*      Author: Skident
*/

#include "SmartFile.h"

using namespace std;

SmartFile::SmartFile()
	: m_sFilename("")
	, m_bCreateIfNotExists(true)
{
	clearCurrMode();
}

SmartFile::SmartFile(const string& __sFileName, ios::openmode __mode, bool __bCreateIfNotExists)
        : m_sFilename(__sFileName)
        , m_mode(__mode)
        , m_bCreateIfNotExists(__bCreateIfNotExists)
{
    open(__sFileName, __mode, __bCreateIfNotExists);
}


SmartFile::~SmartFile()
{
	close();
}


void SmartFile::open(const string& __sFileName, ios::openmode __mode, bool __bCreateIfNotExists)
{
	if (__sFileName.empty())
		throw std::exception("File name can't be empty!");

	if (m_bCreateIfNotExists)
    {
		FSTREAM::open(__sFileName.c_str(), ios::in | ios::out | ios::app); //call the base class method open()
		FSTREAM::close(); 
        m_bCreateIfNotExists = false;
    }

    m_sFilename				= __sFileName;
    m_mode					= __mode;
    m_bCreateIfNotExists	= __bCreateIfNotExists;
    FSTREAM::open(__sFileName.c_str(), __mode); //open file in needed mode
}

void SmartFile::close()
{
	clearCurrMode();
    FSTREAM::close();
}


/**
* @brief	false - if file mode doesn't match the need mode
*			true - current mode is suitable and we don't need to do reopening of file
*/
bool SmartFile::isSuitableMode(const OPEN_MODE& __needMode)
{
    ios::openmode currMode = getCurrMode();

    if ( __needMode & ios::in && !(currMode & ios::in) )
        return false;
    if ( __needMode & ios::out && !(currMode & ios::out) )
        return false;
    if ( __needMode & ios::app && !(currMode & ios::app) )
        return false;
    if ( __needMode & ios::binary && !(currMode & ios::binary) )
        return false;

    return true;
}

bool SmartFile::reopen(const OPEN_MODE& __mode)
{
	if (!isSuitableMode(__mode))
    {
        close();
        open(m_sFilename.c_str(), __mode, true);
		return is_open();
    }

	//if file already opened in need mode then just set read and write cursors at start of file
	return seekToOffset(0);
}

bool SmartFile::seekToOffset(const long& __offset)
{
	if (!is_open())
		return false;
	
	long tellp = 0;
	long tellg = 0;
	OPEN_MODE currMode = getCurrMode();

	//if file opened for read then set up cursor for read to offset dwOffset
	if ( currMode & ios::in )
	{
		FSTREAM::seekg(__offset, ios::beg);
		tellg = FSTREAM::tellg();
	}
	
	//if file opened for write then set up cursor for write to offset dwOffset
	if ( currMode & ios::out )
	{
		FSTREAM::seekp(__offset, ios::beg);
		tellp = FSTREAM::tellp();
	}

	//if tellp or tellg failed(file closed) - reopen file and call this method again (recursion)
	if (tellg == -1 || tellp == -1)
	{
		close(); //close file
		open(m_sFilename, currMode, false); //open file with same mode
		return seekToOffset(__offset);
	}

	if (tellp > tellg)
		return (__offset == tellp);
	return (__offset == tellg);
}

long SmartFile::seekToEnd()
{
	long  tellp = 0;
	long  tellg = 0;
	OPEN_MODE currMode = getCurrMode();


	//if file opened for write then set up cursor for write to offset dwOffset
	if ( currMode & ios::in )
	{
		FSTREAM::seekg(0, ios::end);
		tellg = FSTREAM::tellg();
	}

	//if file opened for write then set up cursor for write to offset dwOffset
	if (currMode & ios::out)
	{
		FSTREAM::seekp(0, ios::end);
		tellp = FSTREAM::tellp();
	}

	//if tellp or tellg failed - reopen file and call this method again
	if (tellg == -1 || tellp == -1)
	{
		close();
		open(m_sFilename, currMode, false);
		return seekToEnd();
	}

	return (tellp > tellg ? tellp : tellg);
}

void SmartFile::clearCurrMode()
{
	m_mode &= ios::in;
	m_mode &= ios::out;
	m_mode &= ios::app;
	m_mode &= ios::binary;
}


inline std::ios::openmode SmartFile::getCurrMode() const 
{ 
	return m_mode; 
}