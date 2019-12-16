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
		long long getValues(long long date, std::vector<long long>* stk, std::vector<double>* openvalue, std::vector<double>* closevalue, StkConContext& ctx) const;

	public:
		//IndexData() : m_dataset(nullptr), m_params(nullptr) {}
		IndexData(const DataSet& data) : m_dataset(&data), m_params(nullptr) {}
		IndexData(const DataSet& data, IndexParameters& params) : m_dataset(&data), m_params(&params) {}
		~IndexData() {}

		//void init(const DataSet& data) { m_dataset = &data; }

		IndexData& simulate(concurrency::cancellation_token* cancel_token = nullptr);
		IndexData& simulate(IndexParameters& params, concurrency::cancellation_token* cancel_token = nullptr) { return setParams(params).simulate(cancel_token); }

		IndexData& setParams(IndexParameters& params) { m_params = &params; return *this; }
		IndexParameters& getParams() { return *m_params; }

		long long getWeight(long long date, std::vector<long long>* stk, std::vector<double>* weight, DataTableColumn type) const;
		long long getOpenWeight(long long date, std::vector<long long>* stk, std::vector<double>* weight) const { return getWeight(date, stk, weight, DataTableColumn::PRE_PRICE); }
		long long getCloseWeight(long long date, std::vector<long long>* stk, std::vector<double>* weight) const { return getWeight(date, stk, weight, DataTableColumn::CLOSE_PRICE); }

		const std::vector<double>& getDailyReturn() const { return m_DailyReturn; }
		const std::vector<double>& getCumReturn() const { return m_CumReturn; }
	};

}
