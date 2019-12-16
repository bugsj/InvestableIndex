#pragma once

#include "conf.h"
#include "DataSet.h"
#include "PeriodEvents.h"
#include "StkPool.h"
#include "SimplePool.h"
#include "SimpleFilePool.h"
#include "MultiSetFilePool.h"
#include "IntersectionPool.h"
#include "SizeTopPool.h"
#include "WeightFactor.h"
#include "TopLimitFactor.h"

namespace InvestableIndex {

	class IndexParameters
	{
	private:
		const DataSet* m_DataSet;
		PeriodEvents m_events;

		long long m_IndexBase = DEFAULT_INDEX_BASE;
		long long m_PeriodStartIndex;
		long long m_PeriodEndIndex;
		int m_FixedDateFlag = 0;
		double m_TopStkWeight = 0;
		long long m_TopStkCnt = 0;
		WeightType m_weighttype = WeightType::FREESHARE;

		MultiSetFilePool* m_section;
		IntersectionPool m_intersectpool;
		SizeTopPool m_toppool;
		const StkPool* m_pool;
		std::vector<SimpleFilePool> m_simplefilepools;
		std::vector<SimplePool> m_simplepools;

		TopLimitFactor m_TopLimitFactor;
		OneWeightFactor m_OneWeightFactor;
		WeightFactor* m_weightfactor;

	public:
		IndexParameters() {}
		IndexParameters(IndexParameters&& o) noexcept :
			m_DataSet(o.m_DataSet),
			m_events(std::move(o.m_events)),
			m_IndexBase(o.m_IndexBase),
			m_PeriodStartIndex(o.m_PeriodStartIndex),
			m_PeriodEndIndex(o.m_PeriodEndIndex),
			m_FixedDateFlag(o.m_FixedDateFlag),
			m_TopStkWeight(o.m_TopStkWeight),
			m_TopStkCnt(o.m_TopStkCnt),
			m_weighttype(o.m_weighttype),
			m_section(o.m_section),
			m_intersectpool(std::move(o.m_intersectpool)),
			m_toppool(std::move(o.m_toppool)),
			m_pool(o.m_pool),
			m_simplefilepools(std::move(o.m_simplefilepools)),
			m_simplepools(std::move(o.m_simplepools)),
			m_TopLimitFactor(std::move(m_TopLimitFactor)),
			m_OneWeightFactor(std::move(m_OneWeightFactor)),
			m_weightfactor(o.m_weightfactor) {

		}

		int init(const DataSet& data);

		long long getDayCount() const { return m_PeriodEndIndex - m_PeriodStartIndex; }
		long long getDateByIndex(long long index) const { return m_DataSet->getDateByIndex(m_PeriodStartIndex + index); }
		long long resetCalendar() { m_PeriodStartIndex = 0; m_PeriodEndIndex = m_DataSet->daycount(); return getDayCount(); }

		long long setIndexBase(long long base) { return m_IndexBase = base; }
		long long getIndexBase() const { return m_IndexBase; }

		WeightType setWeightType(WeightType type) { return m_weighttype = type; }
		WeightType getWeightType() const { return m_weighttype; }

		long long setPeriodStartDate(long long date) { return m_PeriodStartIndex = m_DataSet->getFirstTradeDateIndex(date);; }
		long long getPeriodStartDate() const { return m_DataSet->getDateByIndex(m_PeriodStartIndex); }

		long long setPeriodEndDate(long long date) { return m_PeriodEndIndex = m_DataSet->getLastTradeDateIndex(date); }
		long long getPeriodEndDate() const { return m_DataSet->getDateByIndex(m_PeriodEndIndex); }

		int setFixedDateFlag(int flg) { return flg ? (m_FixedDateFlag = m_events.setEvent(flg)) : (m_FixedDateFlag = 0); }
		int getFixedDateFlag() const { return m_FixedDateFlag; }
		long long getLastEventDate(long long date) const { return m_events.getLastEventDate(date); }

		double setTopStkWeight(double weight) {
			if (weight >= 0.001) { m_weightfactor = &m_TopLimitFactor.setTopLimit(weight); }
			else { m_weightfactor = &m_OneWeightFactor; }
			return m_TopStkWeight = weight;
		}
		double getTopStkWeight() const { return m_TopStkWeight; }

		long long setTopStkCnt(long long cnt) {
			if (cnt > 0) { m_pool = &m_toppool.setTopCount(cnt); }
			else { m_pool = &m_intersectpool; }
			return m_TopStkCnt = cnt;
		}
		long long getTopStkCnt() const { return m_TopStkCnt; }

		const StkPool& setBasePool(const StkPool& base) { return m_intersectpool.setBasePool(base); }
		int setBasePool(int base);
		void removeBasePool() { m_intersectpool.removeBasePool(); }
		void appendPool(StkPool& pool) { m_intersectpool.addPool(pool); }
		long long appendCSVPool(const char* file);
		long long appendSimplePool(long long size, const int* buf);
		void setSectionPool(MultiSetFilePool& sectionpool) { m_section = &sectionpool; }
		long long appendSection(int section);
		void removePools() { m_intersectpool.clearPool(); m_simplefilepools.clear(); m_simplepools.clear(); }
		long long getStkSection(long long date, const std::vector<long long>& stk, std::vector<long long>* section) const {
			return m_DataSet->getStkSection(date, stk, section); 
		}
		long long getShare(long long code, long long date) const {
			return m_DataSet->getShareByType(m_weighttype).getData(code, date);
		}

		const PeriodEvents& getEvents() const { return m_events; }
		const StkPool& getPool() const { 
			DEBUGBREAKCOND(m_pool == nullptr); 
			return *m_pool; 
		}
		const WeightFactor& getWeightFactor() const { 
			DEBUGBREAKCOND(m_weightfactor == nullptr);
			return *m_weightfactor;
		}
	};

}
