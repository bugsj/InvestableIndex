#pragma once
class WeightFactor
{
public:
	WeightFactor();
	virtual ~WeightFactor();

	virtual long long getFactor(long long date, const std::vector<long long> &stk, std::vector<double> *factor) const = 0;
};

class OneWeightFactor :public WeightFactor
{
	virtual long long getFactor(long long date, const std::vector<long long> &stk, std::vector<double> *factor) const {
		factor->resize(stk.size());
		for (auto iter = factor->begin(); iter != factor->end(); ++iter) {
			*iter = 1.0;
		}
		return stk.size();
	}
};
