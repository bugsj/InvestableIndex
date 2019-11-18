#include "pch.h"
#include "IndexParameters.h"

int IndexParameters::init(const DataSet& data)
{
	m_DataSet = &data;

	m_section = 0;
	m_intersectpool.setBasePool(data.getBasePool(3));
	m_pool = &m_intersectpool;
	m_weightfactor = &m_OneWeightFactor;


	m_toppool.init(m_intersectpool, data);
	m_toppool.setTopCount(50);
	m_TopLimitFactor.init(0.1, *this, data);

	m_PeriodStartIndex = 0;
	m_PeriodEndIndex = data.daycount();

	m_events.init(data);

	return  0;
}

int IndexParameters::setBasePool(int base)
{
	if (base == 0) {
		m_intersectpool.removeBasePool();
	}
	else {
		m_intersectpool.setBasePool(m_DataSet->getBasePool(base));
	}
	return base;
}

int IndexParameters::appendSection(int section)
{
	if (m_section == 0) {
		return 0;
	}
	long long rs = m_section->setSection(section);
	if (rs != 0) {
		m_intersectpool.addPool(*m_section);
	}
	return static_cast<int>(rs);
}

int IndexParameters::appendCSVPool(const char* file)
{
	m_simplefilepools.emplace_back(SimpleFilePool());
	long long rs = m_simplefilepools.back().init(file);
	m_intersectpool.addPool(m_simplefilepools.back());
	return static_cast<int>(rs);
}

int IndexParameters::appendSimplePool(int size, const int* buf)
{
	int rs = 0;
	if (size > 0) {
		m_simplepools.emplace_back(SimplePool());
		rs = m_simplepools.back().set(size, buf);
		m_intersectpool.addPool(m_simplepools.back());
	}
	return rs;
}
