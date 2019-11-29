#include "pch.h"

#include "conf.h"

namespace InvestableIndex {
	const TCHAR* STOCK_FREESHARE_FILE = _T("freeshare");
	const TCHAR* STOCK_TOTALSHARE_FILE = _T("totalshare");
	const TCHAR* STOCK_PRICE_FILE = _T("px");
	const TCHAR* STOCK_MARKETCALENDAR_FILE = _T("calendar");

	const TCHAR* INDEX_000300_COM_FILE = _T("idx300");
	const TCHAR* INDEX_000905_COM_FILE = _T("idx500");
	const TCHAR* INDEX_000985_COM_FILE = _T("idx985");
	const TCHAR* INDUSTRY_COM_FILE = _T("sctn");

	const TCHAR* DATAFILE_SUBDIR = _T("iidata");

	const TCHAR* COL_FILE_FORMAT = L"\\%s%s.dat";
	const TCHAR* COL_DATA_FILE_EXT = L".dat";
	const TCHAR* COL_DESC_FILE_EXT = L".tab";
	const int DEFAULT_INDEX_BASE = 1000;
	const TCHAR* DEFAULT_NET_PATH = _T("\\\\172.17.70.100\\部门\\量化投资部\\罗捷\\iidataset");
}
