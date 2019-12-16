#pragma once

namespace InvestableIndex {

	class WeightFactor
	{
	public:
		virtual ~WeightFactor() {}

		virtual long long getFactor(long long date, const std::vector<long long>& stk, std::vector<double>* factor) const = 0;
	};

	class OneWeightFactor :public WeightFactor
	{
	public:
		OneWeightFactor() {}
		OneWeightFactor(const OneWeightFactor& o) {}
		OneWeightFactor(const OneWeightFactor&& o) {}
		virtual long long getFactor(long long date, const std::vector<long long>& stk, std::vector<double>* factor) const {
			DEBUGBREAKCOND(stk.size() == 0);
			factor->resize(stk.size());
			std::fill(factor->begin(), factor->end(), 1.0);
			return factor->size();
		}
	};

}
