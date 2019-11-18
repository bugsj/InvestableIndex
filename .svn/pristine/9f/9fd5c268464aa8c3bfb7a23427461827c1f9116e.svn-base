#include "pch.h"
#include "TopLimitFactor.h"
#include "IndexParameters.h"

TopLimitFactor::TopLimitFactor()
{
	m_dataset = NULL;
	m_params = NULL;
	m_toplimit = 0.1;
}


TopLimitFactor::~TopLimitFactor()
{
}


long long TopLimitFactor::getFactor(long long date, const vector<long long> &stk, vector<double> *factor) const
{
	vector<long long> idxcon;
	m_params->getPool().getSnapshot(date, &idxcon);
	size_t size = idxcon.size();

	factor->resize(size);
	if (size == 0) {
		return size;
	}

	vector<double> mv(size);
	vector<size_t> orderidx(size);

	std::iota(orderidx.begin(), orderidx.end(), 0);
	std::fill(factor->begin(), factor->end(), 1.0);

	double totalmv = 0;
	auto iter_mv = mv.begin();
	const unordered_map<int, int>& stksearcher = m_dataset->getStkIndexOfDay(date);
	for (auto iter = idxcon.begin(); iter != idxcon.end(); ++iter, ++iter_mv) {
		long long index;

		auto stkrs = stksearcher.find(static_cast<const int>(*iter));
		if (stkrs == stksearcher.end()) {
			*iter_mv = 0;
			continue;
		}
		index = stkrs->second;
		*iter_mv = m_dataset->close(index) * m_params->getShare(*iter, date);
		totalmv += *iter_mv;
	}

	if (totalmv == 0) {
		return 0;
	}

	vector<double> mv_rate(size);
	auto rateiter = mv_rate.begin();
	for (iter_mv = mv.begin(); iter_mv != mv.end(); ++iter_mv, ++rateiter) {
		*rateiter = *iter_mv / totalmv;
	}

	std::sort(orderidx.begin(), orderidx.end(), [&mv](size_t i1, size_t i2) { return mv[i1] > mv[i2]; });

	size_t idx = 0;
	double remain = 1.0;
	double adj;
	while (idx < size && mv_rate[orderidx[idx]] > m_toplimit) {
		adj = (remain - m_toplimit) / (remain - mv_rate[orderidx[idx]]);
		mv_rate[orderidx[idx]] = m_toplimit;
		remain -= m_toplimit;
		++idx;
		for (auto iter = orderidx.begin() + idx; iter != orderidx.end(); ++iter) {
			mv_rate[*iter] *= adj;
		}
	}

	// 比率调整完，转化为factor;
	if (idx == size) {
		--idx;
	}
	double ratio_mv = 0, ratio_mvrate = 0;
	for (auto iter = orderidx.begin() + idx; iter != orderidx.end(); ++iter) {
		ratio_mv += mv[*iter];
		ratio_mvrate += mv_rate[*iter];
	}
	double topvalue = ratio_mv / ratio_mvrate * m_toplimit;
	for (auto iter = orderidx.begin(); iter != orderidx.begin() + idx; ++iter) {
		factor->at(*iter) = topvalue / mv[*iter];
	}

	return size;
}