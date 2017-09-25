#pragma once

namespace String
{
	template <typename T>
	static T Remove(const T& str, const T& pattern)
	{
		return Replace<T>(str, pattern, L"");
	}

	template <typename T>
	static T ToLower(const T& Path)
	{
		T r = Path;
		std::transform(r.begin(), r.end(), r.begin(), tolower);
		return r;
	}

	template <typename T>
	static T Replace(const T& _str, const T& token, const T& replacement)
	{
		T str = _str;
		for (;;)
		{
			size_t index = str.find(token.c_str(), 0, token.length());
			if (index == T::npos)
				break;

			str.replace(index, token.length(), replacement);
		}
		return str;
	}
}

