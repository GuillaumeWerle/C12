#include "stdafx.h"
#include "FileSystem.h"
#include "String.h"
#include <minwinbase.h>
#include <shlwapi.h>

#pragma comment(lib, "shlwapi.lib")

using namespace String;

FileSystem * FileSystem::ms_instance = nullptr;

FileSystem::FileSystem(const std::wstring & dataFolder)
{
	Refresh(dataFolder);
}

FileSystem::~FileSystem()
{
}

void FileSystem::Refresh(const std::wstring & dataFolder)
{
	std::lock_guard<std::mutex> lock(m_mutex);

	std::list<std::wstring> files;
	Ls(files, dataFolder, L"*.*", true);
	for (auto & file : files)
	{
		std::wstring filename = PathGetFilename(file);
		m_filenameToPath[filename] = PathNormalize(file);
	}
}

std::wstring FileSystem::PathNormalize(const std::wstring & Path)
{
	std::wstring r = ToLower(Path);
	r = Replace<std::wstring>(r, L"\\", L"/");
	r = Remove<std::wstring>(r, L"./");
	return r;
}

std::wstring FileSystem::PathGetDirectory(const std::wstring & Path)
{
	std::wstring r = PathNormalize(Path);
	size_t pos = r.find_last_of('/');
	if (pos != std::wstring::npos)
	{
		r.erase(pos, r.length());
	}
	return r;
}

std::wstring FileSystem::PathGetFilename(const std::wstring & Path)
{
	std::wstring r = PathNormalize(Path);
	size_t pos = r.find_last_of('/');
	if (pos != std::wstring::npos)
	{
		r.erase(0, pos + 1);
	}
	return r;
}

std::wstring FileSystem::PathNewLine()
{
	wchar_t newline[] = { 0x0D, 0x0A, 0x00 };
	std::wstring nl = newline;
	return nl;
}

std::wstring FileSystem::PathGetFilenameWithoutExtension(const std::wstring & Path)
{
	std::wstring r = PathGetFilename(Path);

	size_t p = r.find_first_of('.');
	if (p != std::wstring::npos)
	{
		r.erase(r.begin() + p, r.end());
	}

	return r;
}


void FileSystem::Ls(std::list<std::wstring> & results, const std::wstring& path, const std::wstring& searchPattern, bool recursive)
{
    WIN32_FIND_DATA return_data;
    HANDLE return_handle = FindFirstFile((path + L"/*.*").c_str(), &return_data);
    if (return_handle != INVALID_HANDLE_VALUE)
    {
        do
        {
            return_data = return_data;

            std::wstring f = return_data.cFileName;

            if (recursive && return_data.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
            {
                if (f != L".." && f != L"." && f != L".svn")
                    Ls(results, path + L"/" + f, searchPattern, recursive);
            }
            else
            {
                if (PathMatchSpec(f.c_str(), searchPattern.c_str()))
                    results.push_back(path + L"/" + f);
            }

        } while (FindNextFile(return_handle, &return_data));

        FindClose(return_handle);
    }
}


bool FileSystem::Find(std::wstring & path, const std::wstring & filename)
{
	std::lock_guard<std::mutex> lock(m_mutex);

	auto it = m_filenameToPath.find(PathNormalize(filename));
	if (it == m_filenameToPath.end())
		return false;

	path = it->second;
	return true;
}

bool FileSystem::ReadFile(std::vector<u8> & data, const Path & path)
{
	HANDLE h = CreateFileW(path.Ptr(), GENERIC_READ, FILE_SHARE_READ, nullptr, OPEN_ALWAYS, FILE_ATTRIBUTE_READONLY, NULL);
	if (h == INVALID_HANDLE_VALUE)
		return false;

	DWORD fileSize = GetFileSize(h, nullptr);
	assert(fileSize);

	data.resize(fileSize);
	DWORD sizeRead;
	::ReadFile(h, &data[0], fileSize, &sizeRead, nullptr);
	assert(sizeRead == fileSize);

	CloseHandle(h);
	return true;
}
