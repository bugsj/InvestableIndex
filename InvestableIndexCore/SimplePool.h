#pragma once

#include "StkPool.h"

namespace InvestableIndex {

	class SimplePool :
		public StkPool
	{
	private:
		std::vector<long long> m_stks;
	public:
		void init() {}
		int set(int size, const int* buf) {
			m_stks.assign(buf, buf + size);
			std::sort(m_stks.begin(), m_stks.end());
			auto last = std::unique(m_stks.begin(), m_stks.end());
			m_stks.erase(last, m_stks.end());
			return static_cast<int>(m_stks.size());
		}
		virtual long long getSnapshot(long long date, std::vector<long long>* list) const {
			list->assign(m_stks.begin(), m_stks.end());
			return list->size();
		}
	};

}
