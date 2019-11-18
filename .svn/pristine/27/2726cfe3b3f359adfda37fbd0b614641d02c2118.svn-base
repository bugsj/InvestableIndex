#include "pch.h"
#include "SizeTopPool.h"

SizeTopPool::SizeTopPool()
{
	m_DataSet = NULL;
}


SizeTopPool::~SizeTopPool()
{
}

long long SizeTopPool::getSnapshot(long long date, vector<long long>* list) const
{
	vector<long long> stks;
	int cnt = static_cast<int>(m_Pool->getSnapshot(date, &stks));
	if (cnt <= m_TopCount) {
		list->swap(stks);
		return cnt;
	}

	vector<double> mv(cnt);
	vector<size_t> orderidx(cnt);

	std::iota(orderidx.begin(), orderidx.end(), 0);

	auto iter_mv = mv.begin();

	const unordered_map<int, int>& stksearcher = m_DataSet->getStkIndexOfDay(date);
	for (auto iter = stks.begin(); iter != stks.end(); ++iter, ++iter_mv) {
		long long index;

		auto stkrs = stksearcher.find(static_cast<const int>(*iter));
		if (stkrs == stksearcher.end()) {
			*iter_mv = 0;
			continue;
		}
		index = stkrs->second;
		*iter_mv = m_DataSet->close(index) * m_DataSet->freeshare(*iter, date);
	}

	auto idxcut = orderidx.begin() + m_TopCount;
	std::nth_element(orderidx.begin(), idxcut, orderidx.end(), [&mv](size_t i1, size_t i2) { return mv[i1] > mv[i2]; });

	list->resize(m_TopCount);
	auto iter_list = list->begin();
	auto iter_idx = orderidx.begin();
	for (int i = m_TopCount; i > 0; --i, ++iter_idx, ++iter_list) {
		*iter_list = stks[*iter_idx];
	}
	std::sort(list->begin(), list->end());
	return m_TopCount;
}
