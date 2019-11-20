#pragma once

#include "DataTable.h"
#include "StkPool.h"

namespace InvestableIndex {

	class SetFilePool :
		public StkPool
	{
	private:
		DataTable m_table;

	public:
		SetFilePool() {}
		~SetFilePool() {}

		const DataTable& getDataTable() const { return m_table; }
		long long init(const TCHAR* file) { return m_table.init(file); }

		virtual long long getSnapshot(long long date, std::vector<long long>* list) const;
	};

}
