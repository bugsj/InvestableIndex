#pragma once
#include "StkPool.h"
#include "IntersectionPool.h"
#include "SizeTopPool.h"
#include "SimplePool.h"
#include "SetFilePool.h"
#include "MultiSetFilePool.h"
#include "SimpleFilePool.h"
#include "TopLimitFactor.h"
#include "WeightFactor.h"
#include "PeriodEvents.h"

enum class WeightType { FREESHARE, TOTALSHARE };

class IndexParameters
{
private:
	const DataSet *m_DataSet;
	PeriodEvents m_events;

	int m_IndexBase = DEFAULT_INDEX_BASE;
	long long m_PeriodStartIndex;
	long long m_PeriodEndIndex;
	int m_FixedDateFlag = 0;
	double m_TopStkWeight = 0;
	int m_TopStkCnt = 0;
	WeightType m_weighttype = WeightType::FREESHARE;

	MultiSetFilePool* m_section;
	IntersectionPool m_intersectpool;
	SizeTopPool m_toppool;
	const StkPool* m_pool;
	vector<SimpleFilePool> m_simplefilepools;
	vector<SimplePool> m_simplepools;

	TopLimitFactor m_TopLimitFactor;
	OneWeightFactor m_OneWeightFactor;
	WeightFactor* m_weightfactor;

public:
	int init(const DataSet& data);

	int getDayCount() const { return static_cast<int>(m_PeriodEndIndex - m_PeriodStartIndex); }
	long long getDateByIndex(long long index) const { return m_DataSet->getDateByIndex(m_PeriodStartIndex + index); }
	int resetCalendar() { m_PeriodStartIndex = 0; m_PeriodEndIndex = m_DataSet->daycount(); return getDayCount(); }

	int setIndexBase(int base) { return m_IndexBase = base; }
	int getIndexBase() const { return m_IndexBase; }

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
		if (weight >= 0.001) {	m_weightfactor = &m_TopLimitFactor.setTopLimit(weight);	} else { m_weightfactor = &m_OneWeightFactor; }
		return m_TopStkWeight = weight;
	}
	double getTopStkWeight() const { return m_TopStkWeight; }

	int setTopStkCnt(int cnt) { 
		if (cnt > 0) { m_pool = &m_toppool.setTopCount(cnt); } else { m_pool = &m_intersectpool; }
		return m_TopStkCnt = cnt; 
	}
	int getTopStkCnt() const { return m_TopStkCnt; }

	const StkPool& setBasePool(const StkPool& base) { return m_intersectpool.setBasePool(base); }
	int setBasePool(int base);
	void removeBasePool() { m_intersectpool.removeBasePool(); }
	void appendPool(StkPool& pool) { m_intersectpool.addPool(pool); }
	int appendCSVPool(const char* file);
	int appendSimplePool(int size, const int *buf);
	void setSectionPool(MultiSetFilePool& sectionpool) { m_section = &sectionpool; }
	int appendSection(int section);
	void removePools() { m_intersectpool.clearPool(); m_simplefilepools.clear(); m_simplepools.clear(); }
	long long getStkSection(long long date, const vector<long long>& stk, vector<long long>* section) const { return m_DataSet->getStkSection(date, stk, section); }
	const StkPool &getPool() const { return *m_pool; }
	long long getShare(long long code, long long date) const { 
		switch (m_weighttype)
		{
		case WeightType::FREESHARE:
			return m_DataSet->freeshare(code, date);
		case WeightType::TOTALSHARE:
			return m_DataSet->totalshare(code, date);
		default:
			return m_DataSet->freeshare(code, date);
		}
	}


	const WeightFactor& getWeightFactor() const { return *m_weightfactor; }
};

