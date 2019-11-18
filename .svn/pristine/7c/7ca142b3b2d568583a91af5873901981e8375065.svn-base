#pragma once
#include "StkPool.h"
#include "DataSet.h"

class SizeTopPool :
	public StkPool
{
private:
	const StkPool* m_Pool = NULL;
	const DataSet* m_DataSet;
	int m_TopCount = 50;
public:
	SizeTopPool();
	~SizeTopPool();

	void init(const StkPool& base, const DataSet& data) { m_Pool = &base; m_DataSet = &data; }
	StkPool& setTopCount(int topcnt) { m_TopCount = topcnt; return *this; }
	StkPool& setPool(StkPool& pool) { m_Pool = &pool; return *this; }

	virtual long long getSnapshot(long long date, std::vector<long long>* list) const;
};

