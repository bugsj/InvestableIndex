#pragma once

#include "conf.h"
#include "DataTable.h"
#include "SetFilePool.h"
#include "MultiSetFilePool.h"

namespace InvestableIndex {

	enum class WeightType { FREESHARE, TOTALSHARE };

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

		const double* m_preprice;
		const double* m_closeprice;

	public:
		static std::unique_ptr<DataSet> create() {
			std::unique_ptr<DataSet> obj(new DataSet());
			obj->init();
			return obj;
		}
		DataSet() :
			m_section(INDUSTRY_COM_FILE),
			m_preprice(nullptr),
			m_closeprice(nullptr)
		{}
		~DataSet() {}

		static void setPath(const TCHAR* pathfmt) { DataTable::setPath(pathfmt); }
		static void setLocal(const TCHAR* path) { DataTable::setLocalPath(path); }
		static void useLocal() { DataTable::useLocal(); }

		long long init();
		long long daycount() const { return m_Calendar.count(); }
		const DataColumn& getCalendarData() const { return m_Calendar.col(0); }
		long long getDateByIndex(long long i) const { return m_Calendar.col(0).getll(i); }

		long long getLastTradeDateIndex(long long date) const;
		long long getFirstTradeDateIndex(long long date) const;
		long long getLastTradeDate(long long date) const;
		long long getFirstTradeDate(long long date) const;

		double price(long long code, long long date, int col) const { return m_StkPrice.col(col).getd(m_StkPrice.getIndexDC(code, date)); }
		double price(long long code, long long date, DataTableColumn col) const { return m_StkPrice.col(col).getd(m_StkPrice.getIndexDC(code, date)); }
		double close(long long code, long long date) const { return price(code, date, DataTableColumn::CLOSE_PRICE /*CLOSEPRICECOL*/); }
		double open(long long code, long long date) const { return price(code, date, DataTableColumn::PRE_PRICE /*OPENPRICECOL*/);; }
		double price(long long index, int col) const { return m_StkPrice.col(col).getd(index); }
		double price(long long index, DataTableColumn col) const { return m_StkPrice.col(col).getd(index); }
		double close(long long index) const { return m_closeprice[index]; }
		double preclose(long long index) const { return m_preprice[index]; }
		long long priceIndex(long long code, long long date) const { return m_StkPrice.getIndexDC(code, date); }
		const std::unordered_map<int, int>& getStkIndexOfDay(long long date) const { return m_StkPrice.getIndexByCodeDay(date); }
		long long freeshare(long long code, long long date) const { return m_FreeShare.getData(code, date); }
		long long totalshare(long long code, long long date) const { return m_TotalShare.getData(code, date); }
		const DataTable& getFreeShare() const { return m_FreeShare; }
		const DataTable& getTotalShare() const { return m_TotalShare; }
		const DataTable& getShareByType(WeightType wt) const { return (wt == WeightType::TOTALSHARE ? m_TotalShare : m_FreeShare); }

		long long calcStkValues(long long date, const std::vector<long long>& code, std::vector<double>* openvalues, std::vector<double>* closevalues, WeightType wt) const;

		const SetFilePool& getBasePool(int base) const;
		long long getStkSection(long long date, const std::vector<long long>& stk, std::vector<long long>* section) const { return m_section.getStkSection(date, stk, section); }
		MultiSetFilePool& getSectionPool() { return m_section; }
	};

}
