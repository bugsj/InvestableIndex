#include "pch.h"

#include "SimpleFilePool.h"

namespace InvestableIndex {

	long long SimpleFilePool::init(const char* file)
	{
		const int string_buf_size = 256;
		char buf[string_buf_size + 1];
		char* buf_ptr;
		buf[string_buf_size] = 0;
		std::ifstream f(file);

		m_stkcode.clear();
		m_entrydate.clear();

		while (f.getline(buf, string_buf_size)) {
			m_stkcode.push_back(atoi(buf));
			buf_ptr = strchr(buf, ',');
			m_entrydate.push_back(atoi(buf_ptr + 1));
		}

		return m_stkcode.size();
	}

	long long SimpleFilePool::getSnapshot(long long date, std::vector<long long>* list) const
	{
		auto x = m_stkcode.begin();
		auto y = m_entrydate.begin();

		for (; x != m_stkcode.end() && y != m_entrydate.end(); ++x, ++y) {
			if (*y <= date) {
				list->push_back(*x);
			}
		}
		return list->size();
	}

}
