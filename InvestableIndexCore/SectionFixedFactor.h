#pragma once

#include "DataSet.h"
#include "TopLimitFactor.h"
#include "MultiSetFilePool.h"

namespace InvestableIndex {

	class IndexParameters;

	class SectionFixedFactor :
		public TopLimitFactor
	{
	private:
		const MultiSetFilePool* m_section;
		std::map<int, double> m_sectionweight;
	public:
		void init(const MultiSetFilePool& section, const DataSet& data, const IndexParameters& params) {
			reinterpret_cast<TopLimitFactor*>(this)->init(0.0, params, data);
			m_section = &section;
		}
		long long setSectionWeight(const std::vector<int>& sections, const std::vector<double>& weights);
		virtual long long getFactor(long long date, const std::vector<long long>& stk, std::vector<double>* factor) const;
	};

}
