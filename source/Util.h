/*
	© 2011 CloudSixteen.com do not share, re-distribute or modify
	this file without the permission of its owner(s).
*/

#ifndef UTIL_H
#define UTIL_H

#include <string>
#include <vector>
#include <sstream>
#include <windows.h>

namespace Util
{
	std::string AbsoluteFilePath();
	std::string WideCharToString(wchar_t* wideChar);
	std::string FixFilePath(const char* fileName, const char* basePath, const char* extension);
	float GetRandom(int min, int max);
	void ExplodeString(std::string text, std::string seperator, std::vector<std::string>* results);
}

#endif