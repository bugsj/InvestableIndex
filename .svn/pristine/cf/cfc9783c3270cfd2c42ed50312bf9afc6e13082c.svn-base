#include "pch.h"
#include "MultiSetFilePool.h"

long long MultiSetFilePool::init(const TCHAR* file)
{
	reinterpret_cast<SetFilePool *>(this)->init(file);

	int cnt = static_cast<int>(getDataTable().count());
	const long long *section = getDataTable().col(STKSECTIONINDEX).getll();
	const long long *stks = getDataTable().col(STKCODEINDEX).getll();
	const long long *exitd = getDataTable().col(STKEXITDATEINDEX).getll();
	int ss;
	int stk;

	for (int i = 0; i < cnt; ++i) {
		unordered_map<int, map<int, int>>::iterator stkiter;

		stk = static_cast<int>(stks[i]);
		ss = static_cast<int>(section[i]);

		stkiter = m_stkindex.find(stk);
		if (stkiter == m_stkindex.end()) {
			m_stkindex.emplace(stk, map<int, int>()).first->second.emplace(static_cast<int>(exitd[i]), ss);
		}
		else {
			stkiter->second.emplace(static_cast<int>(exitd[i]), ss);
		}

		unordered_map<int, vector<int> >::iterator iter;
		
		iter = m_sectionindex.find(ss);
		if (iter == m_sectionindex.end()) {
			m_sectionindex.emplace(ss, vector<int>(1, i));
		}
		else {
			iter->second.push_back(i);
		}

		ss &= 0xffff;
		iter = m_sectionindex.find(ss);
		if (iter == m_sectionindex.end()) {
			m_sectionindex.insert(std::make_pair(ss, vector<int>(1, i)));
		}
		else {
			iter->second.push_back(i);
		}

		ss &= 0xff;
		iter = m_sectionindex.find(ss);
		if (iter == m_sectionindex.end()) {
			m_sectionindex.insert(std::make_pair(ss, vector<int>(1, i)));
		}
		else {
			iter->second.push_back(i);
		}
	}

	return getDataTable().count();
}

long long MultiSetFilePool::getStkSection(long long date, const vector<long long>& stk, vector<long long>* section) const
{
	size_t cnt = stk.size();
	section->resize(cnt);

	auto iter_stk = stk.begin();
	auto iter_sct = section->begin();

	for (; iter_stk != stk.end(); ++iter_stk, ++iter_sct) {
		auto i = m_stkindex.find(static_cast<int>(*iter_stk));
		if (i == m_stkindex.end()) {
			*iter_sct = 0;
			continue;
		}
		*iter_sct = i->second.lower_bound(static_cast<int>(date))->second;
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

	auto current = m_sectionindex.find(static_cast<int>(section));
	if (current == m_sectionindex.end()) {
		return 0;
	}

	m_section = section;
	m_currentsection = &current->second;
	return m_section;
}

long long MultiSetFilePool::getSnapshot(long long date, vector<long long>* list) const
{
	if (m_section == 0) {
		list->clear();
		return 0;
	}

	const long long* refcode = getDataTable().col(STKCODEINDEX).getll();
	const long long* refentrydate = getDataTable().col(STKENTRYDATEINDEX).getll();
	const long long* refexitdate = getDataTable().col(STKEXITDATEINDEX).getll();

	for (auto iter = m_currentsection->begin(); iter != m_currentsection->end(); ++iter) {
		if (refentrydate[*iter] <= date && refexitdate[*iter] >= date) {
			list->push_back(refcode[*iter]);
		}
	}
	return list->size();
}
