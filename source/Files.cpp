/*
	© 2011 CloudSixteen.com do not share, re-distribute or modify
	this file without the permission of its owner(s).
*/

#include "Files.h"
#include "Game.h"

Files::Files() {}

Files::~Files() {}

bool Files::IsSystem(const char* fileName)
{
	if (CL_PathHelp::get_extension(fileName) == "")
		return false;

	CL_VirtualFileSystem& vfs = g_Game->GetVFS();
	CL_VirtualDirectoryListing& vdl = vfs.get_directory_listing(
		CL_PathHelp::get_basepath(fileName)
	);

	std::string newFileName = CL_PathHelp::get_filename(fileName);

	while (vdl.next())
	{
		std::string scanFileName = std::string(vdl.get_filename().c_str());
		if (scanFileName == newFileName)
			return true;
	}

	return false;
}

bool Files::IsLocal(const char* fileName)
{
	return CL_FileHelp::file_exists(fileName);
}

void Files::Delete(const char* fileName)
{
	CL_FileHelp::delete_file(fileName);
}

void Files::Copy(const char* from, const char* to)
{
	CL_FileHelp::copy_file(from, to, true);
}

bool Files::Exists(const char* fileName)
{
	return IsLocal(fileName) || IsSystem(fileName);
}

std::string Files::Read(const char* fileName)
{
	if (!IsLocal(fileName))
	{
		CL_VirtualDirectory& vd = g_Game->GetVD();
		CL_IODevice io = vd.open_file(fileName, CL_File::open_always, CL_File::access_read);

		if (!io.is_null())
			return io.read_string_text(NULL, NULL);
	}

	CL_File file(fileName, CL_File::open_always, CL_File::access_read);
		std::string data = file.read_string_text(NULL, NULL);
	file.close();

	return data;
}

CL_DataBuffer Files::Fetch(const char* fileName)
{
	if (!IsLocal(fileName))
	{
		CL_VirtualDirectory& vd = g_Game->GetVD();
		CL_IODevice io = vd.open_file(fileName, CL_File::open_always, CL_File::access_read);

		if (!io.is_null())
		{
			CL_DataBuffer buffer(io.get_size());
				io.read(buffer.get_data(), buffer.get_size(), true);
			return buffer;
		}
	}

	CL_File file(fileName, CL_File::open_always, CL_File::access_read);
	CL_DataBuffer buffer(file.get_size());
	
	file.read(buffer.get_data(), buffer.get_size(), true);
	file.close();

	return buffer;
}

void Files::Write(const char* fileName, const char* data)
{
	CL_Directory::create(CL_PathHelp::get_basepath(fileName), true);
	CL_File file(fileName, CL_File::create_always, CL_File::access_write);
		file.write_string_text(data);
	file.close();
}

std::string Files::GetExt(const char* fileName)
{
	return CL_PathHelp::get_extension(fileName);
}

std::string Files::GetName(const char* pathName)
{
	return CL_PathHelp::get_filename(pathName);
}

FileScanList& Files::Scan(const char* directory, const char* pattern)
{
	CL_VirtualFileSystem& vfs = g_Game->GetVFS();
	CL_VirtualDirectoryListing& vdl = vfs.get_directory_listing(directory);
		m_lastScan.clear();
	std::map<std::string, bool> m_map;

	while (vdl.next())
	{
		std::string fileName = std::string(vdl.get_filename().c_str());
			m_lastScan.push_back(fileName);
		m_map[fileName] = true;
	}

	if (m_scanner.scan(directory, pattern))
	{
		while (m_scanner.next())
		{
			std::string fileName = std::string(m_scanner.get_name().c_str());
			if (!m_map[fileName])
				m_lastScan.push_back(fileName);
		}
	}

	return m_lastScan;
}

void Files::LuaBind(luabind::object& globals)
{
	luabind::module( g_Lua->State() )
	[
		luabind::class_<Files>("Files")
		.def("IsSystem", &Files::IsSystem)
		.def("IsLocal", &Files::IsLocal)
		.def("Delete", &Files::Delete)
		.def("Copy", &Files::Copy)
		.def("Exists", &Files::Exists)
		.def("Read", &Files::Read)
		.def("Write", &Files::Write)
		.def("GetExt", &Files::GetExt)
		.def("GetName", &Files::GetName)
		.def("Scan", &Files::Scan, luabind::return_stl_iterator)
	];

	globals["FilesInstance"] = g_Files;
}