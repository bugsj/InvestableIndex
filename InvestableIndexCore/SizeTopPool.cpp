#include "pch.h"

#include "SizeTopPool.h"
#include "IndexParameters.h"

namespace InvestableIndex {

	long long SizeTopPool::getSnapshot(long long date, std::vector<long long>* list) const
	{
		std::vector<long long> stks;
		long long cnt = m_Pool->getSnapshot(date, &stks);
		if (cnt <= m_TopCount) {
			list->swap(stks);
			return cnt;
		}

		std::vector<double> mv, mv_open;
		std::vector<size_t> orderidx(cnt);

		std::iota(orderidx.begin(), orderidx.end(), 0);

		m_DataSet->calcStkValues(date, stks, &mv_open, &mv, m_Params->getWeightType());

		auto idxcut = orderidx.begin() + m_TopCount;
		std::nth_element(orderidx.begin(), idxcut, orderidx.end(), [&mv](size_t i1, size_t i2) { return mv[i1] > mv[i2]; });

		list->resize(m_TopCount);
		auto iter_list = list->begin();
		auto iter_idx = orderidx.cbegin();
		for (long long i = m_TopCount; i > 0; --i, ++iter_idx, ++iter_list) {
			*iter_list = stks[*iter_idx];
		}
		std::sort(list->begin(), list->end());
		return m_TopCount;
	}

}
