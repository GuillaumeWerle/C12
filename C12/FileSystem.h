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
	//////////////////////////////////////////////////////////////////////////
	static FileSystem * ms_instance;

	//////////////////////////////////////////////////////////////////////////
	class Path
	{
	public:
		const std::wstring & Str() const { return m_str; }
		const wchar_t * Ptr() const { return m_str.c_str(); }

		Path(const std::wstring & filename)
		{
			InitInternal(filename);
		}
		
		Path(LPCSTR filename)
		{
			std::string s(filename);
			std::wstring w;
			w.assign(s.begin(), s.end());
			InitInternal(w);
		}

	private:
		std::wstring m_str;

		void InitInternal(const std::wstring & filename)
		{
			bool success = FileSystem::ms_instance->Find(m_str, filename);
			if (success == false)
				m_str = L"";
		}
	};

	//////////////////////////////////////////////////////////////////////////
	static std::wstring PathNormalize(const std::wstring & Path);
    static std::wstring PathGetDirectory(const std::wstring & Path);
    static std::wstring PathGetFilename(const std::wstring & Path);
    static std::wstring PathNewLine();
    static std::wstring PathGetFilenameWithoutExtension(const std::wstring & Path);
    static void Ls(std::list<std::wstring> & results, const std::wstring& path, const std::wstring& searchPattern, bool recursive);

	//////////////////////////////////////////////////////////////////////////
	bool Find(std::wstring & path, const std::wstring & filename);

	bool ReadFile(std::vector<u8> & data, const Path & path);

	void Refresh(const std::wstring & dataFolder);

	FileSystem(const std::wstring & dataFolder);
	~FileSystem();

private:
	std::mutex m_mutex;
	std::map<std::wstring, std::wstring> m_filenameToPath;
};

