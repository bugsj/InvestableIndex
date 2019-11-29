#include "pch.h"

#include "tools.h"

namespace InvestableIndex {
	
	int fromExcelDate(int serialdate)
	{
		if (serialdate == 60) {
			return 19000229;
		}
		else if (serialdate < 60)
		{
			++serialdate;
		}

		int year, month, day;
		int l = serialdate + 68569 + 2415019;
		int n = 4 * l / 146097;
		l = l - (146097 * n + 3) / 4;
		int i = 4000 * (l + 1) / 1461001;
		l = l - 1461 * i / 4 + 31;
		int j = 80 * l / 2447;
		day = l - 2447 * j / 80;
		l = j / 11;
		month = j + 2 - 12 * l;
		year = 100 * (n - 49) + i + l;

		return year * 10000 + month * 100 + day;
	}

	int toExcelDate(int digitdate)
	{
		if (digitdate == 19000229) {
			return 60;
		}
		int year = digitdate / 10000;
		int month = (digitdate % 10000) / 100;
		int day = digitdate % 100;
		int serialdate =
			1461 * (year + 4800 + (month - 14) / 12) / 4 +
			367 * (month - 2 - 12 * ((month - 14) / 12)) / 12 -
			3 * ((year + 4900 + (month - 14) / 12) / 100) / 4 +
			day - 2415019 - 32075;
		if (serialdate < 60) {
			--serialdate;
		}
		return serialdate;
	}

	namespace Tools {

		const wchar_t* convMBCS2UNICODE(std::vector<wchar_t>* dst, const char* src)
		{
			int len = CHECK_SIZE_MAXINT32(std::strlen(src));
			int size = ::MultiByteToWideChar(CP_ACP, 0, src, len, NULL, 0) + 1;
			dst->resize(size);
			len = ::MultiByteToWideChar(CP_ACP, 0, src, len, dst->data(), size);

			return dst->data();
		}

		const char* convUNICODE2MBCS(std::vector<char>* dst, const wchar_t* src)
		{
			int len = CHECK_SIZE_MAXINT32(std::wcslen(src));
			int size = ::WideCharToMultiByte(CP_ACP, 0, src, len, NULL, 0, NULL, NULL) + 1;
			dst->resize(size);
			len = ::WideCharToMultiByte(CP_ACP, 0, src, len, dst->data(), size, NULL, NULL);

			return dst->data();
		}

	}
}

