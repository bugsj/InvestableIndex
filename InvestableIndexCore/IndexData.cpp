#include "pch.h"

#include "IndexData.h"
#include "StkConContext.h"

namespace InvestableIndex {

	IndexData& IndexData::simulate(concurrency::cancellation_token* cancel_token)
	{
		int cnt = m_params->getDayCount();
		std::vector<double> dailyreturn(cnt + 1LL);
		std::vector<double> cumreturn(cnt + 1LL);

		dailyreturn[0] = 0;
		cumreturn[0] = m_params->getIndexBase();

		StkConContext ctx(m_params->getEvents(), m_params->getPool(), m_params->getWeightFactor());

		auto dailyprocess = [&](int i) {
			std::vector<long long> stks;
			std::vector<double> stks_closevalue, stks_openvalue;

			long long size = getValues(m_params->getDateByIndex(i), &stks, &stks_openvalue, &stks_closevalue, ctx);

			if (cancel_token && cancel_token->is_canceled()) { return; } // break;

			if (size == 0) {
				dailyreturn[i + 1LL] = 0;
				return; // continue;
			}

			double openvalue = std::accumulate(stks_openvalue.cbegin(), stks_openvalue.cend(), 0.0);
			if (openvalue == 0) {
				dailyreturn[i + 1LL] = 0;
				return; // continue;
			}
			double closevalue = std::accumulate(stks_closevalue.cbegin(), stks_closevalue.cend(), 0.0);

			dailyreturn[i + 1LL] = closevalue / openvalue - 1.0;
		};

		if (cancel_token == nullptr) {
			concurrency::parallel_for(0, cnt, dailyprocess);
		}
		else {
			concurrency::run_with_cancellation_token([&]() {
				concurrency::parallel_for(0, cnt, dailyprocess);
			}, *cancel_token);
		}

		double baseadj = (m_params->getIndexBase() == 0 ? 1.0 : 0.0);
		for (int i = 0; i < cnt; i++) {
			cumreturn[i + 1LL] = (baseadj + cumreturn[i]) * (1.0 + dailyreturn[i + 1LL]) - baseadj;
		}
		if (cancel_token && cancel_token->is_canceled()) {
			concurrency::cancel_current_task();
		}
		dailyreturn.swap(m_DailyReturn);
		cumreturn.swap(m_CumReturn);

		return *this;
	}

	int IndexData::getWeight(long long date, std::vector<long long>* stk, std::vector<double>* weight, int type) const
	{
		stk->clear();
		weight->clear();

		StkConContext ctx(m_params->getPool(), m_params->getWeightFactor());

		std::vector<double> stks_closevalue, stks_openvalue;
		long long size = getValues(date, stk, &stks_openvalue, &stks_closevalue, ctx);

		if (size <= 0) {
			return static_cast<int>(size);
		}

		const std::vector<double>* value = (type == OPENPRICECOL ? &stks_openvalue : &stks_closevalue);

		double mvalue = std::accumulate(value->cbegin(), value->cend(), 0.0);
		auto value_iter = value->cbegin();
		for (auto weight_iter = weight->begin(); weight_iter != weight->end(); ++weight_iter, ++value_iter) {
			*weight_iter = *value_iter / mvalue;
		}

		return static_cast<int>(size);
	}

	int IndexData::getValues(long long date, std::vector<long long>* stk, std::vector<double>* openvalue, std::vector<double>* closevalue, StkConContext& ctx) const
	{
		std::vector<long long> idxcon;
		std::vector<double> conweightfactor;
		std::vector<double> openvalue_raw, closevalue_raw;

		int size = ctx.get(date, &idxcon, &conweightfactor);
		if (size <= 0) {
			return size;
		}

		m_dataset->calcStkValues(date, idxcon, &openvalue_raw, &closevalue_raw, m_params->getWeightType());

		openvalue->resize(size);
		closevalue->resize(size);
		auto openvalue_iter = openvalue->begin();
		auto closevalue_iter = closevalue->begin();
		auto openvalue_raw_iter = openvalue_raw.cbegin();
		auto closevalue_raw_iter = closevalue_raw.cbegin();
		auto wf = conweightfactor.begin();

		for (int i = size; i > 0; --i) {
			*openvalue_iter = *openvalue_raw_iter * *wf;
			*closevalue_iter = *closevalue_raw_iter * *wf;
			++wf, ++openvalue_iter, ++closevalue_iter, ++openvalue_raw_iter, ++closevalue_raw_iter;
		}
		return size;
	}

}
