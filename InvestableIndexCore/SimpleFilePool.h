#pragma once

#include "StkPool.h"

namespace InvestableIndex {

	class SimpleFilePool :
		public StkPool
	{
	private:
		std::vector<long long> m_stkcode;
		std::vector<long long> m_entrydate;
	public:
		SimpleFilePool() {}
		SimpleFilePool(const SimpleFilePool& p) : m_stkcode(p.m_stkcode), m_entrydate(p.m_entrydate) {}
		SimpleFilePool(const SimpleFilePool&& p) : m_stkcode(std::move(p.m_stkcode)), m_entrydate(std::move(p.m_entrydate)) {}
		~SimpleFilePool() {}

		SimpleFilePool(const char* file) { init(file); }
		long long init(const char* file);

		virtual long long getSnapshot(long long date, std::vector<long long>* list) const;
	};

}
