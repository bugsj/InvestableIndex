#include "pch.h"
#include "IndexData.h"
#include "PeriodEvents.h"
#include <ppl.h>

using std::set_intersection;
using std::inserter;

IndexData::IndexData()
{
	m_dataset = NULL;
	m_params = NULL;
}


IndexData::~IndexData()
{
}

void IndexData::init(const DataSet &data)
{
	m_dataset = &data; 
}

IndexData &IndexData::simulate(concurrency::cancellation_token* cancel_token)
{
	int cnt = m_params->getDayCount();
	std::vector<double> dailyreturn(cnt + 1);
	std::vector<double> cumreturn(cnt + 1);

	dailyreturn[0] = 0;
	cumreturn[0] = m_params->getIndexBase();

	auto dailyprocess = [&](int i) {

		double closevalue, openvalue;
		vector<long long> idxcon;
		long long date, refdate;

		date = m_params->getDateByIndex(i);
		refdate = (m_params->getFixedDateFlag() ? m_params->getLastEventDate(date) : date);

		openvalue = 0;
		closevalue = 0;

		idxcon.clear();
		long long size = m_params->getPool().getSnapshot(refdate, &idxcon);

		if (idxcon.empty()) {
			dailyreturn[i + 1] = 0;
			return; // continue;
		}

		// 权重计算被分布在至少3个不同的地方，这里、取权重、权重上限，请统一
		vector<double> conweightfactor;
		m_params->getWeightFactor().getFactor(refdate, idxcon, &conweightfactor);

		const unordered_map<int, int>& stksearcher = m_dataset->getStkIndexOfDay(date);
		auto wf = conweightfactor.begin();
		for (auto iter = idxcon.begin(); iter != idxcon.end(); ++iter, ++wf) {
			if (cancel_token && cancel_token->is_canceled()) {
				return;
			}
			long long shares;
			long long index;
			shares = m_params->getShare(*iter, date);
			auto stkrs = stksearcher.find(static_cast<const int>(*iter));
			if (stkrs == stksearcher.end()) {
				continue;
			}
			index = stkrs->second;
			openvalue += m_dataset->open(index) * shares * *wf;
			closevalue += m_dataset->close(index) * shares * *wf;
		}

		if (openvalue == 0) {
			dailyreturn[i + 1] = 0;
			return; // continue;
		}
		dailyreturn[i + 1] = closevalue / openvalue - 1.0;
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
		cumreturn[i + 1] = (baseadj + cumreturn[i]) * (1.0 + dailyreturn[i + 1]) - baseadj;
	}
	if (cancel_token && cancel_token->is_canceled()) {
		concurrency::cancel_current_task();
	}
	dailyreturn.swap(m_DailyReturn);
	cumreturn.swap(m_CumReturn);

	return *this;
}

int IndexData::getWeight(long long date, vector<long long> *stk, vector<double> *weight, int type) const
{
	stk->clear();
	weight->clear();

	long long refdate;

	refdate = (m_params->getFixedDateFlag() ? m_params->getLastEventDate(date) : date);

	long long size = m_params->getPool().getSnapshot(refdate, stk);
	if (size < 0) {
		return static_cast<int>(size);
	}

	vector<double> conweightfactor;
	m_params->getWeightFactor().getFactor(refdate, *stk, &conweightfactor);

	if (stk->empty()) {
		return 0;
	}

	double mvalue = 0;
	auto wf = conweightfactor.begin();
	weight->resize(stk->size());
	auto iter_weight = weight->begin();
	for (auto iter = stk->begin(); iter != stk->end(); ++iter, ++wf, ++iter_weight) {
		long long shares;
		shares = m_params->getShare(*iter, date);
		*iter_weight = m_dataset->price(*iter, date, type) * shares * *wf;
		mvalue += *iter_weight;
	}
	for (auto iter = weight->begin(); iter != weight->end(); ++iter) {
		*iter /= mvalue;
	}

	return static_cast<int>(stk->size());
}
