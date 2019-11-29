#pragma once

#include "conf.h"
#include "DataColumn.h"
#include "DebugOutput.h"
#include "tools.h"

namespace InvestableIndex {

	class DataTable
	{
	private:
		static std::vector<TCHAR> s_user_path;
		static std::vector<TCHAR> s_defaultlocal_path;
		static std::vector<TCHAR> s_defaultnet_path;
		static const std::vector<TCHAR>* s_path;
		std::vector< std::unique_ptr<DataColumn> > m_columns;
		std::unordered_map<int, std::unordered_map<int, int> > m_index_date;
		std::unordered_map<int, std::map<int, int> > m_index_code;

	private:
		static void getPathFormat(std::vector<TCHAR>* pathfmt) {
			if (s_path->size() > 0) {
				*pathfmt = *s_path;
				Tools::appendStr(pathfmt, COL_FILE_FORMAT);
			}
		}
		std::map<int, int>::const_iterator getLastDateRecord(long long code, long long date) const;

	public:
		DataTable() {}
		~DataTable() {}

		static void setPath(const TCHAR* path = nullptr);
		static void setLocalPath(const TCHAR* path);
		static void useLocal();
		
		long long init(const TCHAR* name);
		long long count() const { return m_columns[0]->count(); }
		const DataColumn& col(int i) const { return *m_columns[i]; }

		void createIndexDate();
		void createIndexCode();

		long long getIndexDC(long long code, long long date) const { return m_index_date.at(CHECK_DATE_MAX(date)).at(CHECK_CODE_MAX(code)); }
		long long getLastDate(long long code, long long date) const { return getLastDateRecord(code, date)->first; }
		long long getLastDateIndex(long long code, long long date) const { return getLastDateRecord(code, date)->second; }
		long long getData(long long code, long long date, int dataindex = STKDATAFIRSTCOL) const {
			return m_columns[dataindex]->getll(getLastDateIndex(code, date));
		}

		const std::unordered_map<int, int>& getIndexByCodeDay(long long date) const { return m_index_date.at(CHECK_DATE_MAX(date)); }
	};

}
