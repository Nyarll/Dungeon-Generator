#pragma once
#include <string>
#include <vector>

class RougeUtils
{
public:
	static std::string CreateString(std::string fmt_str, ...);

	template <typename T>
	static bool Contains(std::vector<T> vec, T value);
};

template<typename T>
inline bool RougeUtils::Contains(std::vector<T> vec, T value)
{
	for (T& element : vec)
	{
		if (element == value)
		{
			return true;
		}
	}
	return false;
}
