#pragma once

#include "StkPool.h"

namespace InvestableIndex {

	class IntersectionPool :
		public StkPool
	{
	private:
		const StkPool* m_basepool;
		std::vector<const StkPool*> m_pools;

	public:
		IntersectionPool() : m_basepool(nullptr) {}
		IntersectionPool(const StkPool* base) : m_basepool(base) {}
		IntersectionPool(const IntersectionPool& p) : m_basepool(p.m_basepool), m_pools(p.m_pools) {}
		IntersectionPool(const IntersectionPool&& p) noexcept: m_basepool(p.m_basepool), m_pools(std::move(p.m_pools)) {}
		~IntersectionPool() {}

		const StkPool& setBasePool(const StkPool& base) { m_basepool = &base; return base; }
		void removeBasePool() { m_basepool = nullptr; }
		void addPool(const StkPool& pool) { m_pools.push_back(&pool); }
		void clearPool() { m_pools.clear(); }

		virtual long long getSnapshot(long long date, std::vector<long long>* list) const;
	};

}
