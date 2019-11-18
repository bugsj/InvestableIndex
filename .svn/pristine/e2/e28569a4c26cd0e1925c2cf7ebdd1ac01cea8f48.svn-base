#pragma once
#include "StkPool.h"
class IntersectionPool :
	public StkPool
{
private:
	const StkPool *m_basepool;
	std::vector<const StkPool *> m_pools; 

public:
	IntersectionPool();
	~IntersectionPool();

	const StkPool &setBasePool(const StkPool &base) { m_basepool = &base; return base; }
	void removeBasePool() { m_basepool = NULL; }
	void addPool(const StkPool &pool) { m_pools.push_back(&pool); }
	void clearPool() { m_pools.clear(); }

	virtual long long getSnapshot(long long date, std::vector<long long>* list) const;
};

