#pragma once

#include "conf.h"
#include "DataColumn.h"

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
			const TCHAR* fmt = COL_FILE_FORMAT;
			size_t fmtdatalen = wcslen(fmt) + 1;
			if (s_path->size() > 0) {
				*pathfmt = *s_path;
				size_t len = wcslen(pathfmt->data());
				pathfmt->resize(len + fmtdatalen + 1);
				::CopyMemory(pathfmt->data() + len, fmt, sizeof(*fmt) * fmtdatalen);
			}
		}

	public:
		DataTable() {}
		~DataTable() {}

		static void setPath(const TCHAR* path = nullptr) {
			if (path != nullptr) {
				size_t pathlen = wcslen(path);
				s_user_path.resize(pathlen + 1);
				::CopyMemory(s_user_path.data(), path, sizeof(*path) * static_cast<int>(pathlen + 1));
			}
		}
		static void setLocalPath(const TCHAR* path) {
			if (path != nullptr) {
				size_t pathlen = wcslen(path);
				s_defaultlocal_path.resize(pathlen + 1);
				::CopyMemory(s_user_path.data(), path, sizeof(*path) * static_cast<int>(pathlen + 1));
			}
		}
		
		int init(const TCHAR* name);
		long long count() const { return m_columns[0]->count(); }
		const DataColumn& col(int i) const { return *m_columns[i]; }

		void createIndexDate();
		void createIndexCode();

		long long getIndexDC(long long code, long long date) const { return m_index_date.at(static_cast<int>(date)).at(static_cast<int>(code)); }
		long long getLastDate(long long code, long long date) const;
		long long getLastDateIndex(long long code, long long date) const;
		long long getData(long long code, long long date, int dataindex = STKDATAFIRSTCOL) const {
			return m_columns[dataindex]->getll(getLastDateIndex(code, date));
		}

		const std::unordered_map<int, int>& getIndexByCodeDay(long long date) const { return m_index_date.at(static_cast<int>(date)); }
	};

}
