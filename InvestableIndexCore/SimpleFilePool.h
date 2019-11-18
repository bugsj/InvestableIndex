#pragma once
#include "StkPool.h"
class SimpleFilePool :
	public StkPool
{
private:
	std::vector<long long> m_stkcode;
	std::vector<long long> m_entrydate;
public:
	SimpleFilePool();
	~SimpleFilePool();

	long long init(const char *file);

	virtual long long getSnapshot(long long date, std::vector<long long> *list) const;
};

