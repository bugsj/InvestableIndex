#pragma once
#include "TopLimitFactor.h"
#include "DataSet.h"
#include "IndexParameters.h"
#include "MultiSetFilePool.h"

class SectionFixedFactor :
	public TopLimitFactor
{
private:
	const MultiSetFilePool* m_section;
	std::map<int, double> m_sectionweight;
public:
	void init(const MultiSetFilePool& section, const DataSet& data, const IndexParameters& params) {
		reinterpret_cast<TopLimitFactor *>(this)->init(0.0, params, data);
		m_section = &section;
	}
	long long setSectionWeight(const vector<int>& sections, const vector<double>& weights);
	virtual long long getFactor(long long date, const std::vector<long long>& stk, std::vector<double>* factor) const;
};
