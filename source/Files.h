/*
	© 2011 CloudSixteen.com do not share, re-distribute or modify
	this file without the permission of its owner(s).
*/

#ifndef FILES_H
#define FILES_H

#include <ClanLib/core.h>
#include "Lua.h"
#include <list>

typedef std::list<std::string> FileScanList;

class Files : public CNonCopyable
{
public:
	bool IsSystem(const char* fileName);
	bool IsLocal(const char* fileName);
	void Delete(const char* fileName);
	void Copy(const char* from, const char* to);
	bool Exists(const char* fileName);
	std::string Read(const char* fileName);
	std::string GetExt(const char* fileName);
	std::string GetName(const char *pathName);
	FileScanList& Scan(const char* directory, const char* pattern);
	void Write(const char* fileName, const char* data);
	CL_DataBuffer Fetch(const char* fileName);
	void LuaBind(luabind::object& globals);
	~Files();
	Files();
private:
	CL_DirectoryScanner m_scanner;
	FileScanList m_lastScan;
};

#define g_Files CSingleton<Files>::Instance()

#endif