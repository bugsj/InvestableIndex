#pragma once

#include "WeightFactor.h"

namespace InvestableIndex {

	class DataSet;
	class IndexParameters;

	class TopLimitFactor :
		public WeightFactor
	{
	private:
		double m_toplimit;
		const DataSet* m_dataset;
		const IndexParameters* m_params;
	public:
		TopLimitFactor() : m_dataset(nullptr), m_params(nullptr), m_toplimit(0.1) {}
		TopLimitFactor(double top, const IndexParameters& params, const DataSet& data) : m_dataset(&data), m_params(&params), m_toplimit(top) {}
		TopLimitFactor(const TopLimitFactor& f) : m_dataset(f.m_dataset), m_params(f.m_params), m_toplimit(f.m_toplimit) {}
		TopLimitFactor(const TopLimitFactor&& f) : m_dataset(f.m_dataset), m_params(f.m_params), m_toplimit(f.m_toplimit) {}
		~TopLimitFactor() {}

		void init(double top, const IndexParameters& index, const DataSet& data) { m_toplimit = top; m_params = &index; m_dataset = &data; }
		TopLimitFactor& setTopLimit(double top) { m_toplimit = top; return *this; }
		double getTopLimit() const { return m_toplimit; }
		const DataSet& getDataSet() const { return *m_dataset; }
		const IndexParameters& getIndexParameters() const { return *m_params; }
		virtual long long getFactor(long long date, const std::vector<long long>& stk, std::vector<double>* factor) const;
	};

}
