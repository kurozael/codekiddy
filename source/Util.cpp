/*
	© 2011 CloudSixteen.com do not share, re-distribute or modify
	this file without the permission of its owner(s).
*/

#include "Util.h"
#include "Files.h"
#include <string>

namespace Util
{
	std::string AbsoluteFilePath()
	{
		wchar_t pathBuffer[2048];
		std::string filePath;

		GetModuleFileName(NULL, pathBuffer, 2048);

		filePath = WideCharToString(pathBuffer);
		filePath = filePath.substr(0, filePath.rfind("\\") + 1);

		return filePath;
	}

	std::string FixFilePath(const char* fileName, const char* basePath, const char* extension)
	{
		if ( !g_Files->Exists(fileName) )
		{
			std::string fileNameString(fileName);
			std::string basePathString(basePath);
			std::string extensionString(extension);

			if (fileNameString.substr( 0, basePathString.length() ) != basePathString)
				fileNameString = basePathString + fileNameString;

			if (g_Files->GetExt(fileName) == "")
				fileNameString.append(extensionString);
			
			return fileNameString;
		}

		return std::string(fileName);
	}

	std::string WideCharToString(wchar_t* wideChar)
	{
		std::wstring wideString(wideChar);
		return std::string( wideString.begin(), wideString.end() );
	}

	float GetRandom(int min, int max)
	{
		return ( (float( rand() ) / float(RAND_MAX) ) * (max - min)) + min;
	}

	void ExplodeString(std::string text, std::string separator, std::vector<std::string>* results)
	{
		int found = found = text.find_first_of(separator);

		while (found != std::string::npos)
		{
			if (found > 0)
			{
				results->push_back(text.substr(0,found));
			}

			text = text.substr(found + 1);
			found = text.find_first_of(separator);
		}

		if (text.length() > 0)
		{
			results->push_back(text);
		}
	}
}