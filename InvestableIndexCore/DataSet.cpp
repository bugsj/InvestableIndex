#include "pch.h"

#include "DataSet.h"

namespace InvestableIndex {

	long long DataSet::init()
	{
		CHECK_DATAFILE_OPEN(m_FreeShare.init(STOCK_FREESHARE_FILE), STOCK_FREESHARE_FILE);
		CHECK_DATAFILE_OPEN(m_TotalShare.init(STOCK_TOTALSHARE_FILE), STOCK_TOTALSHARE_FILE);
		CHECK_DATAFILE_OPEN(m_StkPrice.init(STOCK_PRICE_FILE), STOCK_PRICE_FILE);
		CHECK_DATAFILE_OPEN(m_Calendar.init(STOCK_MARKETCALENDAR_FILE), STOCK_MARKETCALENDAR_FILE);

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

	long long DataSet::getLastTradeDateIndex(long long date) const
	{
		const long long* begin = m_Calendar.col(0).getll();
		const long long* end = begin + m_Calendar.count();

		const long long* pos = std::find_if(begin, end, std::bind2nd(std::greater<long long>(), date));
		if (pos > begin) {
			--pos;
		}
		return pos - begin;
	}

	long long DataSet::getFirstTradeDateIndex(long long date) const
	{
		const long long* begin = m_Calendar.col(0).getll();
		const long long* end = begin + m_Calendar.count();

		const long long* pos = std::find_if_not(begin, end, std::bind2nd(std::less<long long>(), date));

		return pos - begin;
	}

	long long DataSet::getLastTradeDate(long long date) const
	{
		const long long* begin = m_Calendar.col(0).getll();
		const long long* end = begin + m_Calendar.count();

		const long long* pos = std::find_if(begin, end, std::bind2nd(std::greater<long long>(), date));
		if (pos > begin) {
			--pos;
		}
		return *pos;
	}

	long long DataSet::getFirstTradeDate(long long date) const
	{
		const long long* begin = m_Calendar.col(0).getll();
		const long long* end = begin + m_Calendar.count();

		const long long* pos = std::find_if_not(begin, end, std::bind2nd(std::less<long long>(), date));

		return *pos;
	}

	const SetFilePool& DataSet::getBasePool(int base) const
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

	long long DataSet::calcStkValues(long long date, const std::vector<long long>& code
		, std::vector<double>* openvalues, std::vector<double>* closevalues, WeightType wt) const
	{
		long long size = code.size();
		
		if (size <= 0) {
			return size;
		}

		const DataTable& share_table = getShareByType(wt);
		const std::unordered_map<int, int>& stksearcher = getStkIndexOfDay(date);
		openvalues->resize(size);
		closevalues->resize(size);
		auto openvalue_iter = openvalues->begin();
		auto closevalue_iter = closevalues->begin();
		for (auto iter :code) {
			long long shares;
			long long index;
			shares = share_table.getData(iter, date);
			auto stkrs = stksearcher.find(CHECK_CODE_MAX(iter));
			if (stkrs == stksearcher.end()) {
				continue;
			}
			index = stkrs->second;
			*openvalue_iter++ = open(index) * shares;
			*closevalue_iter++ = close(index) * shares;
		}
		return size;
	}

}
