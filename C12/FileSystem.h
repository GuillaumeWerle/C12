#pragma once

class FilePath
{
public:
    std::wstring m_path;
protected:
private:
};

class FileSystem
{
public:

    std::wstring Remove(const std::wstring & inputString, wchar_t * pattern)
    {
        size_t buffSize = (inputString.size() + 1) * sizeof(wchar_t);
        wchar_t * buff = (wchar_t *)alloca(buffSize);
        *buff = 0;
        wcscat_s(buff, buffSize, inputString.c_str());

        for (;;)
        {
            wchar_t * toremove = ::wcsstr(buff, pattern);
            if (toremove == nullptr)
                break;
            wcscpy_s(toremove, wcslen(toremove), toremove + wcslen(pattern));
        }

        return std::wstring(buff);
    }


    std::wstring PathNormalize(const std::wstring & Path)
    {
        std::wstring r = ToLower(Path);
        r = Replace(r, '\\', '/');
        r = Remove(r, L"./");
        return r;
    }


    std::wstring PathGetDirectory(const std::wstring & Path)
    {
        std::wstring r = PathNormalize(Path);
        size_t pos = r.find_last_of('/');
        if (pos != std::wstring::npos)
        {
            r.erase(pos, r.length());
        }
        return r;
    }

    std::wstring PathGetFilename(const std::wstring & Path)
    {
        std::wstring r = PathNormalize(Path);
        size_t pos = r.find_last_of('/');
        if (pos != std::wstring::npos)
        {
            r.erase(0, pos + 1);
        }
        return r;
    }

    std::wstring ToLower(const std::wstring & Path)
    {
        std::wstring r = Path;
        std::transform(r.begin(), r.end(), r.begin(), tolower);
        return r;
    }

    std::wstring Replace(const std::wstring & Path, char o, char n)
    {
        std::wstring r = Path;
        size_t p = 0;
        for (;;)
        {
            p = r.find_first_of(o, p);
            if (p == std::wstring::npos)
                break;
            r[p] = n;
        }

        return r;
    }

    void Replace(std::wstring& str, const std::wstring& token, const std::wstring& replacement)
    {
        if (token.empty())
            return;
        size_t start_pos = 0;
        while ((start_pos = str.find(token, start_pos)) != std::wstring::npos) {
            str.replace(start_pos, token.length(), replacement);
            start_pos += replacement.length(); // In case 'to' contains 'from', like replacing 'x' with 'yx'
        }
    }

    std::wstring PathNewLine()
    {
        wchar_t newline[] = { 0x0D, 0x0A, 0x00 };
        std::wstring nl = newline;
        return nl;
    }

    std::wstring PathGetFilenameWithoutExtension(const std::wstring & Path)
    {
        std::wstring r = PathGetFilename(Path);

        size_t p = r.find_first_of('.');
        if (p != std::wstring::npos)
        {
            r.erase(r.begin() + p, r.end());
        }

        return r;
    }


    void Ls(std::list<std::wstring> & results, const std::wstring& path, const std::wstring& searchPattern, bool recursive);

    FileSystem();
    ~FileSystem();
};

