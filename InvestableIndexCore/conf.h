#pragma once

namespace InvestableIndex {
	extern const TCHAR* STOCK_FREESHARE_FILE;
	extern const TCHAR* STOCK_TOTALSHARE_FILE;
	extern const TCHAR* STOCK_PRICE_FILE;
	extern const TCHAR* STOCK_MARKETCALENDAR_FILE;

	extern const TCHAR* INDEX_000300_COM_FILE;
	extern const TCHAR* INDEX_000905_COM_FILE;
	extern const TCHAR* INDEX_000985_COM_FILE;
	extern const TCHAR* INDUSTRY_COM_FILE;

	extern const TCHAR* DATAFILE_SUBDIR;

	constexpr int FILE_DATA_DEFAULT_WIDTH = 8;

	constexpr int STKCODEINDEX = 0;
	constexpr int STKDATEINDEX = 1;
	constexpr int STKDATAFIRSTCOL = 2;
	constexpr int STKSHARECOL = 2;
	constexpr int STKENTRYDATEINDEX = 1;
	constexpr int STKEXITDATEINDEX = 2;
	constexpr int STKSECTIONINDEX = 3;
	constexpr int OPENPRICECOL = 2;
	constexpr int CLOSEPRICECOL = 3;

	extern const TCHAR* COL_FILE_FORMAT;
	extern const TCHAR* COL_DATA_FILE_EXT;
	extern const TCHAR* COL_DESC_FILE_EXT;

	extern const int DEFAULT_INDEX_BASE;
	extern const TCHAR* DEFAULT_NET_PATH;

	constexpr int EST_TRADINGDAY_PER_YEAR = 256;
	constexpr int EST_DATA_COUNT_YEARS = 12;
	constexpr int EST_MARKET_STOCK_COUNT = 4096;
}
