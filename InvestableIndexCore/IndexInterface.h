#pragma once

#include "test.h"
#include "IndexParameters.h"
#include <vector>
#include <map>
#include <atomic>
#include <ppltasks.h>

class IndexInterface
{
private:
	DataSet m_dataset;
	static HMODULE m_module;

	std::vector<IndexParameters> m_params_pool;
	std::vector<IndexData> m_index_pool;
	IndexParameters *m_params = 0;
	IndexData *m_index = 0;

	int m_current_params = 0;
	int m_current_index = 0;

	IndexInterface();
	bool m_initflag = false;
	std::atomic_flag m_atomic_init_flag = ATOMIC_FLAG_INIT;
	HANDLE m_initevent;
public:
	static void setModuleHandle(HMODULE hInst) { m_module = hInst; }
	static void setPath(const TCHAR* path) { DataSet::setPath(path); }
	static IndexInterface* get() {
		static IndexInterface II_index;
		if (II_index.m_initflag == false) {
			II_index.init();
		}
		return &II_index;
	}

	void init();

	int getDayCount() const { return m_params->getDayCount(); }
	long long getDateByIndex(long long index) const { return m_params->getDateByIndex(index); }
	WeightType setWeightType(WeightType type) { return m_params->setWeightType(type); }
	WeightType getWeightType() const { return m_params->getWeightType(); }
	int setIndexBase(int base) { return m_params->setIndexBase(base); }
	int getIndexBase() const { return m_params->getIndexBase(); }
	long long setPeriodStartDate(long long date) { return m_params->setPeriodStartDate(date); }
	long long getPeriodStartDate() const { return m_params->getPeriodStartDate(); }
	long long setPeriodEndDate(long long date) { return m_params->setPeriodEndDate(date); }
	long long getPeriodEndDate() const { return m_params->getPeriodEndDate(); }
	void setFixedDate(int flag) { m_params->setFixedDateFlag(flag); }
	void setTopStkWeight(int limit) { m_params->setTopStkWeight(limit / 100.0); }
	void setTopStkCount(int cnt) { m_params->setTopStkCnt(cnt); }
	int setIndexBasePool(int base) { return m_params->setBasePool(base); }
	void removePools() { m_params->removePools(); }
	int appendCSVPool(const char* file) { return m_params->appendCSVPool(file); }
	int appendSimplePool(int size, const int* buf) { return m_params->appendSimplePool(size, buf); }
	int appendSectionPool(int section) { return m_params->appendSection(section); }
	
	long long getLastTradeDate(long long date) { return m_dataset.getLastTradeDate(date); }

	IndexData &simulate(concurrency::cancellation_token *token = nullptr) { return m_index->simulate(*m_params, token); }
	int getOpenWeight(long long date, vector<long long>* stks, vector<double>* weight) const { return m_index->getOpenWeight(date, stks, weight); }
	int getOpenSectionWeight(long long date, vector<long long>* section, vector<double>* weight, int level) const;
	const vector<double>& getDailyReturn() const { return m_index->getDailyReturn(); }
	const vector<double>& getCumReturn() const { return m_index->getCumReturn(); }

	void writeFileReturnDaily(const char* file) const;
	void writeFileReturnYearly(const char* file) const;
	void writeFileWeightDay(const char* file, long long date) const;

	void writeMapReturnDaily(std::map<int, double>* r) const;

	void test() const { ::test(*m_index, m_dataset); }

};

