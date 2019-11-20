#include "pch.h"

#include "StkConContext.h"

namespace InvestableIndex {

	int StkConContext::get(int date, std::vector<long long>* stks, std::vector<double>* factor)
	{
		if (m_events == nullptr) {
			int size = static_cast<int>(m_pool->getSnapshot(date, stks));
			m_factor->getFactor(date, *stks, factor);
			return size;
		}
		else {
			std::lock_guard<std::mutex> lck(m_get_mtx);
			int refdate = m_events->getLastEventDate(date);
			auto iter = m_cache.find(refdate);
			if (iter != m_cache.end()) {
				*stks = iter->second.first;
				*factor = iter->second.second;
				return stks->size();
			}
			else {
				int size = static_cast<int>(m_pool->getSnapshot(refdate, stks));
				m_factor->getFactor(refdate, *stks, factor);
				m_cache.emplace(refdate, make_pair(*stks, *factor));
				return size;
			}
		}
		return 0;
	}

}