#pragma once

#include <type_traits>
#include "StkPool.h"

namespace InvestableIndex {

	class SimplePool :
		public StkPool
	{
	private:
		std::vector<long long> m_stks;
	public:
		SimplePool() {}
		SimplePool(const SimplePool& p) : m_stks(p.m_stks) {}
		SimplePool(const SimplePool&& p) : m_stks(std::move(p.m_stks)) {}
		void init() {}

		template<typename T>
		long long set(long long size, const T* buf) {
			static_assert(std::is_integral<T>::value, "integer only");
			m_stks.assign(buf, buf + size);
			std::sort(m_stks.begin(), m_stks.end());
			auto last = std::unique(m_stks.begin(), m_stks.end());
			m_stks.erase(last, m_stks.end());
			return m_stks.size();
		}

		virtual long long getSnapshot(long long date, std::vector<long long>* list) const {
			list->assign(m_stks.begin(), m_stks.end());
			return list->size();
		}
	};

}
