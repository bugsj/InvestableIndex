#pragma once

#include "conf.h"
#include "DataColumn.h"
#include "DebugOutput.h"
#include "tools.h"

namespace InvestableIndex {
	enum class DataTableColumn : long long {
		STK_CODE = 0,
		STK_DATE,
		STK_SHARE,
		STK_ENTRY_DATE,
		STK_EXIT_DATE,
		STK_SECTION,
		PRE_PRICE,
		//OPEN_PRICE,
		CLOSE_PRICE,
		ColumnIndex_LAST_VALUE
	};

	constexpr long long MAX_COLUMN_INDEX() {
		return static_cast<long long>(DataTableColumn::ColumnIndex_LAST_VALUE);
	}

	DataTableColumn DataTableColumnMap(const char* col_name);

	class DataTable
	{
	private:
		static std::vector<TCHAR> s_user_path;
		static std::vector<TCHAR> s_defaultlocal_path;
		static std::vector<TCHAR> s_defaultnet_path;
		static const std::vector<TCHAR>* s_path;
		std::vector<DataColumn> m_columns;
		std::unordered_map<int, std::unordered_map<int, int> > m_index_date;
		std::unordered_map<int, std::map<int, int> > m_index_code;

		std::vector<long long> m_columns_map = std::vector<long long>(MAX_COLUMN_INDEX(), -1LL);

	private:
		std::map<int, int>::const_iterator getLastDateRecord(long long code, long long date) const;
		long long getColumnIndex(DataTableColumn column) const {
			DEBUGBREAKCOND(column == DataTableColumn::ColumnIndex_LAST_VALUE);
			return m_columns_map[static_cast<long long>(column)];
		}
		DataTable& setColumnIndex(DataTableColumn column, long long index) {
			if (column != DataTableColumn::ColumnIndex_LAST_VALUE) {
				m_columns_map[static_cast<long long>(column)] = index;
			}
			return *this;
		}
		DataTable& setColumnMap(const char* field_name, long long index) {
			return setColumnIndex(DataTableColumnMap(field_name), index);
		}

	public:
		DataTable() {}
		~DataTable() {}

		DataTable(const TCHAR* name) { init(name); }

		static const TCHAR* getPath() { return s_path->data(); }
		static void setPath(const TCHAR* path = nullptr) {
			if (path != nullptr) {
				s_user_path.clear();
				Tools::appendStr(&s_user_path, path);
			}
		}
		static void setLocalPath(const TCHAR* path) {
			if (path != nullptr) {
				s_defaultlocal_path.clear();
				Tools::appendStr(&s_defaultlocal_path, path);
			}
		}
		static void useLocal();
		
		long long init(const TCHAR* name);
		long long count() const { return m_columns[0].count(); }
		const inline DataColumn& col(int i) const { return m_columns[i]; }
		const inline DataColumn& col(DataTableColumn c) const { 
			long long i = getColumnIndex(c);
			DEBUGBREAKCOND(i == -1);
			return m_columns[i]; 
		}

		void createIndexDate();
		void createIndexCode();

		long long getIndexDC(long long code, long long date) const { return m_index_date.at(CHECK_DATE_MAX(date)).at(CHECK_CODE_MAX(code)); }
		long long getLastDate(long long code, long long date) const { return getLastDateRecord(code, date)->first; }
		long long getLastDateIndex(long long code, long long date) const { return getLastDateRecord(code, date)->second; }
		long long getData(long long code, long long date, int dataindex = STKDATAFIRSTCOL) const {
			return m_columns[dataindex].getll(getLastDateIndex(code, date));
		}

		const std::unordered_map<int, int>& getIndexByCodeDay(long long date) const { return m_index_date.at(CHECK_DATE_MAX(date)); }
	};

}
