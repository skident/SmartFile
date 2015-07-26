/*
 * SmartFile.h
 *
 *  Created on: 7.11.2014
 *      Author: Skident
 */

#pragma once

#include <fstream>
//#include <boost/filesystem/fstream.hpp>
#include <string>

typedef std::fstream FSTREAM;
//typedef boost::filesystem::fstream FSTREAM;
typedef std::ios::openmode OPEN_MODE;

class SmartFile: public FSTREAM
{
private:
	std::string				m_sFilename;
	std::ios::openmode		m_mode;
	bool					m_bCreateIfNotExists;

public:
	SmartFile();
	SmartFile(const std::string& __sFileName, OPEN_MODE __mode = std::ios::in | std::ios::out, bool __bCreateIfNotExists = true);
	virtual  ~SmartFile();

	void open(const std::string& __sFileName, OPEN_MODE __mode = std::ios::in | std::ios::out, bool __bCreateIfNotExists = true);
	bool reopen(const OPEN_MODE& __mode);
	void close();

	void clearCurrMode();
	inline OPEN_MODE getCurrMode() const;

	bool isSuitableMode(const OPEN_MODE& __mode);
	bool seekToOffset(const long& __offset);
	long seekToEnd();
};

