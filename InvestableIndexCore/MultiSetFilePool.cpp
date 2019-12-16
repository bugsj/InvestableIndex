#include "pch.h"

#include "MultiSetFilePool.h"

namespace InvestableIndex {

	long long MultiSetFilePool::createIndex()
	{
		long long cnt = m_table.count();
		const long long* section = m_table.col(STKSECTIONINDEX).getll();
		const long long* stks = m_table.col(STKCODEINDEX).getll();
		const long long* exitd = m_table.col(STKEXITDATEINDEX).getll();
		
		for (int i = 0; i < cnt; ++i) {
			std::unordered_map<int, std::map<int, int>>::iterator stkiter;

			int stk = CHECK_CODE_MAX(stks[i]);
			int ss = CHECK_CODE_MAX(section[i]);

			stkiter = m_stkindex.find(stk);
			if (stkiter == m_stkindex.end()) {
				m_stkindex.emplace(stk, std::map<int, int>()).first->second.emplace(CHECK_DATE_MAX(exitd[i]), ss);
			}
			else {
				stkiter->second.emplace(CHECK_DATE_MAX(exitd[i]), ss);
			}

			std::unordered_map<int, std::vector<int> >::iterator iter;

			iter = m_sectionindex.find(ss);
			if (iter == m_sectionindex.end()) {
				m_sectionindex.emplace(ss, std::vector<int>(1, i));
			}
			else {
				iter->second.push_back(i);
			}

			ss &= 0xffff;
			iter = m_sectionindex.find(ss);
			if (iter == m_sectionindex.end()) {
				m_sectionindex.insert(std::make_pair(ss, std::vector<int>(1, i)));
			}
			else {
				iter->second.push_back(i);
			}

			ss &= 0xff;
			iter = m_sectionindex.find(ss);
			if (iter == m_sectionindex.end()) {
				m_sectionindex.insert(std::make_pair(ss, std::vector<int>(1, i)));
			}
			else {
				iter->second.push_back(i);
			}
		}

		return m_table.count();
	}

	long long MultiSetFilePool::getStkSection(long long date, const std::vector<long long>& stk, std::vector<long long>* section) const
	{
		size_t cnt = stk.size();
		section->resize(cnt);

		auto iter_stk = stk.begin();
		auto iter_sct = section->begin();

		for (; iter_stk != stk.end(); ++iter_stk, ++iter_sct) {
			auto i = m_stkindex.find(CHECK_CODE_MAX(*iter_stk));
			if (i == m_stkindex.end()) {
				*iter_sct = 0;
				continue;
			}
			*iter_sct = i->second.lower_bound(CHECK_DATE_MAX(date))->second;
		}

		return section->size();
	}

	long long MultiSetFilePool::setSection(long long section)
	{
		if (section == 0) {
			m_section = section;
			m_currentsection = NULL;
			return 0;
		}

		auto current = m_sectionindex.find(CHECK_CODE_MAX(section));
		if (current == m_sectionindex.end()) {
			return 0;
		}

		m_section = section;
		m_currentsection = &current->second;
		return m_section;
	}

	long long MultiSetFilePool::getSnapshot(long long date, std::vector<long long>* list) const
	{
		if (m_section == 0) {
			list->clear();
			return 0;
		}

		const long long* refcode = m_table.col(STKCODEINDEX).getll();
		const long long* refentrydate = m_table.col(STKENTRYDATEINDEX).getll();
		const long long* refexitdate = m_table.col(STKEXITDATEINDEX).getll();

		for (auto iter :*m_currentsection) {
			if (refentrydate[iter] <= date && refexitdate[iter] >= date) {
				list->push_back(refcode[iter]);
			}
		}
		return list->size();
	}

}
