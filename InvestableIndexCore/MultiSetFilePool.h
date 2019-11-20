#pragma once

#include "SetFilePool.h"

namespace InvestableIndex {

	class MultiSetFilePool :
		public SetFilePool
	{
	private:
		long long m_section = 0;
		std::unordered_map<int, std::vector<int> > m_sectionindex;
		std::unordered_map<int, std::map<int, int>> m_stkindex;
		std::vector<int>* m_currentsection = NULL;
	public:
		long long init(const TCHAR* file);
		virtual long long getSnapshot(long long date, std::vector<long long>* list) const;

		long long setSection(long long section);
		long long getStkSection(long long date, const std::vector<long long>& stk, std::vector<long long>* section) const;
	};

}
