#pragma once

#define WIN32_LEAN_AND_MEAN             // 从 Windows 头文件中排除极少使用的内容
// Windows 头文件
#include <windows.h>
#include <tchar.h>


#include <intrin.h>

#ifdef _DEBUG

#define DEBUGBREAK  __debugbreak()
#define DEBUGBREAKCOND(expr) do { if (expr) __debugbreak(); } while(0)

#else

#define DEBUGBREAK  ((void)0)
#define DEBUGBREAKCOND(expr)  ((void)0)

#endif


inline int CHECK_SIZE_MAXINT32(long long n)
{
	if (n > MAXINT32) {
		throw std::length_error("size too big: maxint32");
	}
	return static_cast<int>(n);
}

inline int CHECK_VALUE_MAXINT32(long long n)
{
	if (n > MAXINT32) {
		throw std::overflow_error("value too big: maxint32");
	}
	return static_cast<int>(n);
}

inline unsigned int CHECK_SIZE_MAXUINT32(long long n)
{
	if (n > MAXUINT32) {
		throw std::length_error("size too big: maxuint32");
	}
	return static_cast<unsigned int>(n);
}

inline int CHECK_DATE_MAX(long long n)
{
#ifdef _DEBUG
	if (n > 99999999) {
		throw std::overflow_error("date too big: 99999999");
	}
#endif
	return static_cast<int>(n);
}

inline int CHECK_CODE_MAX(long long n)
{
#ifdef _DEBUG
	if (n > 99999999) {
		throw std::overflow_error("code too big: 99999999");
	}
#endif
	return static_cast<int>(n);
}

inline void CHECK_DATAFILE_OPEN(long long cnt, const TCHAR *file = nullptr) 
{
	if (cnt <= 0) {
		throw std::runtime_error("data file failed");
	}
}
