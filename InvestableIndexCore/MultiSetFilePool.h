#pragma once

#include "DataTable.h"
#include "StkPool.h"

namespace InvestableIndex {

	class MultiSetFilePool : public StkPool
	{
	private:
		DataTable m_table;
		long long m_section = 0;
		std::unordered_map<int, std::vector<int> > m_sectionindex;
		std::unordered_map<int, std::map<int, int>> m_stkindex;
		std::vector<int>* m_currentsection = nullptr;

	private:
		long long createIndex();

	public:
		MultiSetFilePool(const TCHAR* file) : m_table(file) { createIndex(); }
		virtual long long getSnapshot(long long date, std::vector<long long>* list) const;

		long long setSection(long long section);
		long long getStkSection(long long date, const std::vector<long long>& stk, std::vector<long long>* section) const;
	};

}
