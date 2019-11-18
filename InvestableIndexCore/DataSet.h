#pragma once
#include "DataTable.h"
#include "DataColumn.h"
#include "SetFilePool.h"
#include "MultiSetFilePool.h"

using std::find_if;
using std::find_if_not;
using std::bind2nd;
using std::greater;
using std::less;
using std::set;


class DataSet
{
private:
	DataTable m_FreeShare;
	DataTable m_TotalShare;
	DataTable m_StkPrice;
	DataTable m_Calendar;

	SetFilePool m_idx300;
	SetFilePool m_idx905;
	SetFilePool m_idx985;
	MultiSetFilePool m_section;

public:
	DataSet();
	~DataSet();

	static void setPath(const TCHAR *pathfmt) { DataTable::setPath(pathfmt); }
	static void setLocal(const TCHAR *path) { DataTable::setLocalPath(path); }

	int init();
	int daycount() const { return static_cast<int>(m_Calendar.count()); }
	const DataColumn &getCalendarData() const{ return m_Calendar.col(0); }
	long long getDateByIndex(long long i) const { return m_Calendar.col(0).getll(i); }

	int getLastTradeDateIndex(long long date) const;
	int getFirstTradeDateIndex(long long date) const;
	long long getLastTradeDate(long long date) const;
	long long getFirstTradeDate(long long date) const;

	double price(long long code, long long date, int col) const { return m_StkPrice.col(col).getll(m_StkPrice.getIndexDC(code, date)) / 10000.0; }
	double close(long long code, long long date) const { return price(code, date, CLOSEPRICECOL); }
	double open(long long code, long long date) const { return price(code, date, OPENPRICECOL);; }
	double price(long long index, int col) const { return m_StkPrice.col(col).getll(index) / 10000.0; }
	double close(long long index) const { return price(index, CLOSEPRICECOL); }
	double open(long long index) const { return price(index, OPENPRICECOL); }
	long long priceIndex(long long code, long long date) const { return m_StkPrice.getIndexDC(code, date); }
	const unordered_map<int, int>& getStkIndexOfDay(long long date) const { return m_StkPrice.getIndexByCodeDay(date); }
	long long freeshare(long long code, long long date) const { return m_FreeShare.col(2).getll(m_FreeShare.getLastDateIndex(code, date)); }
	long long totalshare(long long code, long long date) const { return m_TotalShare.col(2).getll(m_TotalShare.getLastDateIndex(code, date)); }

	const SetFilePool &getBasePool(int base) const;
	long long getStkSection(long long date, const vector<long long>& stk, vector<long long>* section) const { return m_section.getStkSection(date, stk, section); }
	MultiSetFilePool &getSectionPool() { return m_section; }
};
