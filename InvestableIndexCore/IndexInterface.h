#pragma once

#include "DataSet.h"
#include "IndexData.h"
#include "test.h"

namespace InvestableIndex {

	class IndexInterface
	{
	private:
		std::unique_ptr<DataSet> m_dataset;
		static HMODULE m_module;

		std::vector<IndexParameters> m_params_pool;
		std::vector<IndexData> m_index_pool;
		IndexParameters* m_params = 0;
		IndexData* m_index = 0;

		int m_current_params = 0;
		int m_current_index = 0;

		IndexInterface() {}
		bool m_initflag = false;
		std::mutex m_init_mtx;
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
		IndexInterface(const IndexInterface& o) = delete;
		IndexInterface& operator=(const IndexInterface& o) = delete;

		void init();

		long long getDayCount() const { return m_params->getDayCount(); }
		long long getDateByIndex(long long index) const { return m_params->getDateByIndex(index); }
		WeightType setWeightType(WeightType type) { return m_params->setWeightType(type); }
		WeightType getWeightType() const { return m_params->getWeightType(); }
		long long setIndexBase(long long base) { return m_params->setIndexBase(base); }
		long long getIndexBase() const { return m_params->getIndexBase(); }
		long long setPeriodStartDate(long long date) { return m_params->setPeriodStartDate(date); }
		long long getPeriodStartDate() const { return m_params->getPeriodStartDate(); }
		long long setPeriodEndDate(long long date) { return m_params->setPeriodEndDate(date); }
		long long getPeriodEndDate() const { return m_params->getPeriodEndDate(); }
		void setFixedDate(int flag) { m_params->setFixedDateFlag(flag); }
		void setTopStkWeight(long long limit) { m_params->setTopStkWeight(limit / 100.0); }
		void setTopStkCount(long long cnt) { m_params->setTopStkCnt(cnt); }
		int setIndexBasePool(int base) { return m_params->setBasePool(base); }
		void removePools() { m_params->removePools(); }
		long long appendCSVPool(const char* file) { return m_params->appendCSVPool(file); }
		long long appendSimplePool(long long size, const int* buf) { return m_params->appendSimplePool(size, buf); }
		long long appendSectionPool(int section) { return m_params->appendSection(section); }

		long long getLastTradeDate(long long date) { return m_dataset->getLastTradeDate(date); }

		IndexData& simulate(concurrency::cancellation_token* token = nullptr) { return m_index->simulate(*m_params, token); }
		long long getOpenWeight(long long date, std::vector<long long>* stks, std::vector<double>* weight) const { return m_index->getOpenWeight(date, stks, weight); }
		long long getOpenSectionWeight(long long date, std::vector<long long>* section, std::vector<double>* weight, long long level) const;
		const std::vector<double>& getDailyReturn() const { return m_index->getDailyReturn(); }
		const std::vector<double>& getCumReturn() const { return m_index->getCumReturn(); }

		void writeFileReturnDaily(const char* file) const;
		void writeFileReturnYearly(const char* file) const;
		void writeFileWeightDay(const char* file, long long date) const;

		void writeMapReturnDaily(std::map<int, double>* r) const;

		void test() const { InvestableIndex::test(*m_index, *m_dataset); }

	};

}
