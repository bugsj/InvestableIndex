#include "pch.h"
#include "DataSet.h"

#include <ppl.h>

DataSet::DataSet()
{
}


DataSet::~DataSet()
{
}

int DataSet::init()
{
	m_FreeShare.init(InvestableIndex::STOCK_FREESHARE_FILE);
	m_TotalShare.init(InvestableIndex::STOCK_TOTALSHARE_FILE);
	m_StkPrice.init(InvestableIndex::STOCK_PRICE_FILE);
	m_Calendar.init(InvestableIndex::STOCK_MARKETCALENDAR_FILE);

	concurrency::parallel_invoke(
		[&] { m_FreeShare.createIndexCode(); },
		[&] { m_FreeShare.createIndexDate(); },
		[&] { m_TotalShare.createIndexCode(); },
		[&] { m_TotalShare.createIndexDate(); },
		[&] { m_StkPrice.createIndexCode(); },
		[&] { m_StkPrice.createIndexDate(); }
	);

	m_idx300.init(INDEX_000300_COM_FILE);
	m_idx905.init(INDEX_000905_COM_FILE);
	m_idx985.init(INDEX_000985_COM_FILE);
	m_section.init(INDUSTRY_COM_FILE);
	
	return daycount();
}

int DataSet::getLastTradeDateIndex(long long date) const
{
	const long long *begin = m_Calendar.col(0).getll();
	const long long *end = begin + m_Calendar.count();

	const long long *pos = find_if(begin, end, bind2nd(greater<long long>(), date));
	if (pos > begin) { 
		--pos; 
	}
	return static_cast<int>(pos - begin);
}

int DataSet::getFirstTradeDateIndex(long long date) const
{
	const long long *begin = m_Calendar.col(0).getll();
	const long long *end = begin + m_Calendar.count();

	const long long *pos = find_if_not(begin, end, bind2nd(less<long long>(), date));

	return static_cast<int>(pos - begin);
}

long long DataSet::getLastTradeDate(long long date) const
{
	const long long* begin = m_Calendar.col(0).getll();
	const long long* end = begin + m_Calendar.count();

	const long long* pos = find_if(begin, end, bind2nd(greater<long long>(), date));
	if (pos > begin) {
		--pos;
	}
	return *pos;
}

long long DataSet::getFirstTradeDate(long long date) const
{
	const long long* begin = m_Calendar.col(0).getll();
	const long long* end = begin + m_Calendar.count();

	const long long* pos = find_if_not(begin, end, bind2nd(less<long long>(), date));

	return *pos;
}

const SetFilePool &DataSet::getBasePool(int base) const
{
	switch (base) {
	case 3:
		return m_idx300;
	case 5:
		return m_idx905;
	case 9:
		return m_idx985;
	default:
		return m_idx300;
	}
}