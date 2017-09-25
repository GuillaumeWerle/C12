#include "stdafx.h"
#include "FileSystem.h"
#include <minwinbase.h>
#include <shlwapi.h>

#pragma comment(lib, "shlwapi.lib")

FileSystem::FileSystem()
{
}


FileSystem::~FileSystem()
{
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

