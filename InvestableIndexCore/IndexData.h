#pragma once
#include "IndexParameters.h"
#include <ppltasks.h>

class IndexData
{
private:
	const DataSet *m_dataset;
	IndexParameters* m_params;

	vector<double> m_DailyReturn;
	vector<double> m_CumReturn;

public:
	IndexData();
	~IndexData();

	void init(const DataSet &data);

	IndexData &simulate(concurrency::cancellation_token* cancel_token = nullptr);
	IndexData &simulate(IndexParameters &params, concurrency::cancellation_token* cancel_token = nullptr) { return setParams(params).simulate(cancel_token); }

	IndexData &setParams(IndexParameters& params) { m_params = &params; return *this; }
	IndexParameters &getParams() { return *m_params; }

	int getWeight(long long date,vector<long long> *stk, vector<double> *weight, int type) const;
	int getOpenWeight(long long date, vector<long long> *stk, vector<double> *weight) const { return getWeight(date, stk, weight, OPENPRICECOL); }
	int getCloseWeight(long long date, vector<long long> *stk, vector<double> *weight) const { return getWeight(date, stk, weight, CLOSEPRICECOL); }

	const vector<double> &getDailyReturn() const { return m_DailyReturn; }
	const vector<double> &getCumReturn() const { return m_CumReturn; }
};

