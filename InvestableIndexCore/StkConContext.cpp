#include "pch.h"

#include "StkConContext.h"

namespace InvestableIndex {

	StkConContext::StkConContext(const PeriodEvents& events, const StkPool& pool, const WeightFactor& factor)
		:m_events(&events), m_pool(&pool), m_factor(&factor) 
	{
		for (auto d : events.getEvents()) {
			std::vector<long long> stks;
			std::vector<double> factors;
			long long size = m_pool->getSnapshot(d, &stks);
			if (size > 0) {
				m_factor->getFactor(d, stks, &factors);
				m_cache.emplace(CHECK_DATE_MAX(d), make_pair(stks, factors));
			}
		}
	}

	long long StkConContext::get(long long date, std::vector<long long>* stks, std::vector<double>* factor) const
	{
		if (m_events) {
			long long refdate = m_events->getLastEventDate(date);
			auto iter = m_cache.find(CHECK_DATE_MAX(refdate));
			if (iter != m_cache.end()) {
				*stks = iter->second.first;
				*factor = iter->second.second;
				return stks->size();
			}
		}

		long long size = m_pool->getSnapshot(date, stks);
		m_factor->getFactor(date, *stks, factor);
		return size;
	}

}