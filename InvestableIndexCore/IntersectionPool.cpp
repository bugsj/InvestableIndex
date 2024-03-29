#include "pch.h"

#include "IntersectionPool.h"

namespace InvestableIndex {

	long long IntersectionPool::getSnapshot(long long date, std::vector<long long>* list) const
	{
		if (m_basepool == NULL && m_pools.empty()) {
			return ERR_NOPOOLS;
		}

		std::vector<long long> second;
		std::vector<long long> result;

		if (m_basepool) {
			m_basepool->getSnapshot(date, list);

			for (auto iter :m_pools) {
				second.clear();
				iter->getSnapshot(date, &second);

				result.clear();
				std::set_intersection(list->cbegin(), list->cend(), second.cbegin(), second.cend(), std::inserter(result, result.begin()));

				list->swap(result);
			}
		}
		else {
			m_pools.front()->getSnapshot(date, list);

			for (auto iter = m_pools.begin() + 1; iter != m_pools.end(); ++iter) {
				second.clear();
				(*iter)->getSnapshot(date, &second);

				result.clear();
				set_intersection(list->begin(), list->end(), second.begin(), second.end(), inserter(result, result.begin()));

				list->swap(result);
			}
		}

		return list->size();
	}

}
