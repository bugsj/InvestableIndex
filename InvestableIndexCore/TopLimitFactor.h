#pragma once
#include "WeightFactor.h"
#include "DataSet.h"

class DataSet;
class IndexParameters;

class TopLimitFactor :
	public WeightFactor
{
private:
	double m_toplimit;
	const DataSet *m_dataset;
	const IndexParameters *m_params;
public:
	TopLimitFactor();
	~TopLimitFactor();

	void init(double top, const IndexParameters& index, const DataSet &data) { m_toplimit = top; m_params = &index; m_dataset = &data; }
	TopLimitFactor &setTopLimit(double top) { m_toplimit = top; return *this; }
	double getTopLimit() const { return m_toplimit; }
	const DataSet &getDataSet() const { return *m_dataset; }
	const IndexParameters &getIndexParameters() const { return *m_params; }
	virtual long long getFactor(long long date, const vector<long long> &stk, vector<double> *factor) const;
};

