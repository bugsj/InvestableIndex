#include "pch.h"

#include "DataTable.h"
#include "tools.h"

namespace InvestableIndex {

	std::vector<TCHAR> DataTable::s_user_path;
	std::vector<TCHAR> DataTable::s_defaultlocal_path;
	std::vector<TCHAR> DataTable::s_defaultnet_path = std::vector<TCHAR>(DEFAULT_NET_PATH, DEFAULT_NET_PATH + wcslen(DEFAULT_NET_PATH) + 1);
	const std::vector<TCHAR>* DataTable::s_path = &DataTable::s_defaultnet_path;

	static inline long long mergecd(long long code, long long date)
	{
		return (code << 32) + date;
	}

	void DataTable::setPath(const TCHAR* path) {
		if (path != nullptr) {
			Tools::appendStr(&s_user_path, path);
		}
	}
	void DataTable::setLocalPath(const TCHAR* path) {
		if (path != nullptr) {
			Tools::appendStr(&s_defaultlocal_path, path);
		}
	}
	void DataTable::useLocal() {
		if (!s_defaultlocal_path.empty()) {
			std::vector<TCHAR> testfile(s_defaultlocal_path);
			Tools::appendStr(&testfile, { L"\\", STOCK_MARKETCALENDAR_FILE, COL_DESC_FILE_EXT });

			Debug::WriteLog(testfile.data());

			DWORD attr = GetFileAttributes(testfile.data());
			if (attr != INVALID_FILE_ATTRIBUTES) {
				s_path = &s_defaultlocal_path;
			}
		}
	}

	long long DataTable::init(const TCHAR* name)
	{
		if (name == nullptr) {
			return 0;
		}

		std::vector<WCHAR> tabfile;
		Tools::appendStr(&tabfile, { s_path->data(), L"\\", name, COL_DESC_FILE_EXT });

		FILE* fp;
		if ((fp = ::_wfsopen(tabfile.data(), L"r", _SH_DENYNO)) == NULL) {
			return 0;
		}

		std::vector<std::vector<WCHAR> > colfile;
		std::vector<int> colwidth;

		const int bufsize = 256;
		char buf[bufsize];
		while (fgets(buf, bufsize, fp)) {
			char* ptr = buf;
			while (*ptr != ',' && *ptr != '\0' && *ptr != '\n' && ptr - buf < bufsize) {
				++ptr;
			}
			int width;
			if (*ptr == ',' && (width = atoi(ptr + 1)) > 0) {
				colwidth.push_back(width);
			}
			else {
				colwidth.push_back(FILE_DATA_DEFAULT_WIDTH);
			}
			*ptr = '\0';
			colfile.emplace_back(std::vector<WCHAR>());
			Tools::convMBCS2UNICODE(&colfile.back(), buf);
			DWORD err = ::GetLastError();
		}
		::fclose(fp);

		std::vector<TCHAR> datafile;
		for (unsigned int i = 0; i < colfile.size(); ++i) {
			datafile.clear();
			Tools::appendStr(&datafile, { s_path->data(), _T("\\"), name, colfile[i].data(), COL_DATA_FILE_EXT });
			m_columns.emplace_back(std::make_unique<DataColumn>());
			CHECK_DATAFILE_OPEN(m_columns.back()->init(datafile.data(), colwidth[i]), datafile.data());
		}
		return colfile.size();
	}

	void DataTable::createIndexDate()
	{
		const long long* refcode = m_columns[STKCODEINDEX]->getll();
		const long long* refcodeend = m_columns[STKCODEINDEX]->getll() + m_columns[STKCODEINDEX]->count();
		const long long* refdate = m_columns[STKDATEINDEX]->getll();
		int index = 0;

		m_index_date.reserve(EST_TRADINGDAY_PER_YEAR * EST_DATA_COUNT_YEARS);
		m_index_date.rehash(EST_TRADINGDAY_PER_YEAR * EST_DATA_COUNT_YEARS);

		while (refcode != refcodeend) {
			std::unordered_map<int, std::unordered_map<int, int> >::iterator iter;
			iter = m_index_date.find(CHECK_DATE_MAX(*refdate));
			if (iter == m_index_date.end()) {
				iter = m_index_date.emplace(CHECK_DATE_MAX(*refdate), std::unordered_map<int, int>()).first;
				iter->second.reserve(EST_MARKET_STOCK_COUNT);
				iter->second.rehash(EST_MARKET_STOCK_COUNT);
			}
			iter->second.emplace(CHECK_CODE_MAX(*refcode), index);
			++refcode;
			++refdate;
			++index;
		}
	}


	void DataTable::createIndexCode()
	{
		const long long* refcode = m_columns[STKCODEINDEX]->getll();
		const long long* refcodeend = m_columns[STKCODEINDEX]->getll() + m_columns[STKCODEINDEX]->count();
		const long long* refdate = m_columns[STKDATEINDEX]->getll();
		int index = 0;

		while (refcode != refcodeend) {
			std::unordered_map<int, std::map<int, int> >::iterator iter;
			iter = m_index_code.find(CHECK_CODE_MAX(*refcode));
			if (iter == m_index_code.end()) {
				iter = m_index_code.emplace(CHECK_CODE_MAX(*refcode), std::map<int, int>()).first;
			}
			iter->second.emplace(CHECK_DATE_MAX(*refdate), index);
			++refcode;
			++refdate;
			++index;
		}
	}

	std::map<int, int>::const_iterator DataTable::getLastDateRecord(long long code, long long date) const
	{
		const std::map<int, int>& datelist = m_index_code.at(CHECK_CODE_MAX(code));
		std::map<int, int>::const_iterator iter = datelist.upper_bound(CHECK_DATE_MAX(date));
		if (iter == datelist.cbegin()) {
			return iter;
		}
		return --iter;
	}

}
