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

		SetFilePool(const TCHAR* file) : m_table(file) {}
		long long init(const TCHAR* file) { return m_table.init(file); }
		virtual long long getSnapshot(long long date, std::vector<long long>* list) const;
	};

}
