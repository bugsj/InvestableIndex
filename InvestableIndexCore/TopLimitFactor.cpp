#include "pch.h"

#include "TopLimitFactor.h"

#include "DataSet.h"
#include "IndexParameters.h"

namespace InvestableIndex {

	long long TopLimitFactor::getFactor(long long date, const std::vector<long long>& stk, std::vector<double>* factor) const
	{
		std::vector<double> openmv, mv;
		size_t size = m_dataset->calcStkValues(date, stk, &openmv, &mv, m_params->getWeightType());

		factor->resize(size);
		if (size <= 0) {
			return size;
		}
		std::vector<size_t> orderidx(size);

		std::iota(orderidx.begin(), orderidx.end(), 0);
		std::fill(factor->begin(), factor->end(), 1.0);

		double totalmv = std::accumulate(mv.begin(), mv.end(), 0.0);
		if (totalmv == 0) {
			return 0;
		}

		std::vector<double> mv_rate(size);
		auto rateiter = mv_rate.begin();
		for (auto iter_mv :mv) {
			*rateiter = iter_mv / totalmv;
			++rateiter;
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
			for (auto iter = orderidx.cbegin() + idx; iter != orderidx.cend(); ++iter) {
				mv_rate[*iter] *= adj;
			}
		}

		// 比率调整完，转化为factor;
		if (idx == size) {
			--idx;
		}
		double ratio_mv = 0, ratio_mvrate = 0;
		for (auto iter = orderidx.cbegin() + idx; iter != orderidx.cend(); ++iter) {
			ratio_mv += mv[*iter];
			ratio_mvrate += mv_rate[*iter];
		}
		double topvalue = ratio_mv / ratio_mvrate * m_toplimit;
		for (auto iter = orderidx.cbegin(); iter != orderidx.cbegin() + idx; ++iter) {
			factor->at(*iter) = topvalue / mv[*iter];
		}

		return size;
	}

}
