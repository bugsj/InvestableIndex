#pragma once

#include "PeriodEvents.h"
#include "StkPool.h"
#include "WeightFactor.h"

namespace InvestableIndex {

	class StkConContext
	{
	private:
		std::mutex m_get_mtx;
		std::map<int, std::pair< std::vector<long long>, std::vector<double> > > m_cache;

		const PeriodEvents* m_events;
		const StkPool* m_pool;
		const WeightFactor* m_factor;

	public:
		StkConContext(const PeriodEvents& events, const StkPool& pool, const WeightFactor& factor) :
			m_events(&events), m_pool(&pool), m_factor(&factor) {}
		StkConContext(const StkPool& pool, const WeightFactor& factor) :
			m_events(nullptr), m_pool(&pool), m_factor(&factor) {}

		int get(int date, std::vector<long long>* stks, std::vector<double>* factor);
	};

}
