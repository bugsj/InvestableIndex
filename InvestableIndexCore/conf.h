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

	const int FILE_DATA_DEFAULT_WIDTH = 8;

	const int STKCODEINDEX = 0;
	const int STKDATEINDEX = 1;
	const int STKDATAFIRSTCOL = 2; 
	const int STKSHARECOL = 2;
	const int STKENTRYDATEINDEX = 1;
	const int STKEXITDATEINDEX = 2;
	const int STKSECTIONINDEX = 3;
	const int OPENPRICECOL = 2;
	const int CLOSEPRICECOL = 3;

	extern const TCHAR* COL_FILE_FORMAT;
	extern const TCHAR* COL_DATA_FILE_EXT;
	extern const TCHAR* COL_DESC_FILE_EXT;

	extern const int DEFAULT_INDEX_BASE;
	extern const TCHAR* DEFAULT_NET_PATH;

	const int EST_TRADINGDAY_PER_YEAR = 256;
	const int EST_DATA_COUNT_YEARS = 12;
	const int EST_MARKET_STOCK_COUNT = 4096;
}
