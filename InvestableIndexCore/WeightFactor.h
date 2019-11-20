#pragma once

namespace InvestableIndex {

	class WeightFactor
	{
	public:
		WeightFactor() {}
		virtual ~WeightFactor() {}

		virtual long long getFactor(long long date, const std::vector<long long>& stk, std::vector<double>* factor) const = 0;
	};

	class OneWeightFactor :public WeightFactor
	{
		virtual long long getFactor(long long date, const std::vector<long long>& stk, std::vector<double>* factor) const {
			factor->resize(stk.size());
			std::fill(factor->begin(), factor->end(), 1.0);
			return factor->size();
		}
	};

}
