#pragma once

#include "conf.h"
#include "IndexParameters.h"

namespace InvestableIndex {

	class StkConContext;

	class IndexData
	{
	private:
		const DataSet* m_dataset;
		IndexParameters* m_params;

		std::vector<double> m_DailyReturn;
		std::vector<double> m_CumReturn;

	private:
		int getValues(long long date, std::vector<long long>* stk, std::vector<double>* openvalue, std::vector<double>* closevalue, StkConContext& ctx) const;

	public:
		IndexData() : m_dataset(nullptr), m_params(nullptr) {}
		~IndexData() {}

		void init(const DataSet& data) { m_dataset = &data; }

		IndexData& simulate(concurrency::cancellation_token* cancel_token = nullptr);
		IndexData& simulate(IndexParameters& params, concurrency::cancellation_token* cancel_token = nullptr) { return setParams(params).simulate(cancel_token); }

		IndexData& setParams(IndexParameters& params) { m_params = &params; return *this; }
		IndexParameters& getParams() { return *m_params; }

		int getWeight(long long date, std::vector<long long>* stk, std::vector<double>* weight, int type) const;
		int getOpenWeight(long long date, std::vector<long long>* stk, std::vector<double>* weight) const { return getWeight(date, stk, weight, OPENPRICECOL); }
		int getCloseWeight(long long date, std::vector<long long>* stk, std::vector<double>* weight) const { return getWeight(date, stk, weight, CLOSEPRICECOL); }

		const std::vector<double>& getDailyReturn() const { return m_DailyReturn; }
		const std::vector<double>& getCumReturn() const { return m_CumReturn; }
	};

}
