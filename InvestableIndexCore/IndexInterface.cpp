#include "pch.h"

#include "IndexInterface.h"

namespace InvestableIndex {

	HMODULE IndexInterface::m_module = NULL;

	void IndexInterface::init()
	{
		std::lock_guard<std::mutex> lck(m_init_mtx);

		if (!m_initflag) {
			m_dataset.init();

			m_params_pool.emplace_back(IndexParameters());
			m_params = &m_params_pool.back();
			m_params->init(m_dataset);
			m_params->setSectionPool(m_dataset.getSectionPool());

			m_index_pool.emplace_back(IndexData());
			m_index = &m_index_pool.back();
			m_index->init(m_dataset);

			m_initflag = true;
		}
	}

	int IndexInterface::getOpenSectionWeight(long long date, std::vector<long long>* section, std::vector<double>* weight, int level) const
	{
		std::vector<long long> stks;
		std::vector<double> stkweight;
		std::vector<long long> stksection;

		unsigned long long masks[] = { 0, 0xff, 0xffff, 0xffffff };
		unsigned long long sectionmask = masks[level];

		m_index->getOpenWeight(date, &stks, &stkweight);
		m_params->getStkSection(date, stks, &stksection);

		std::map<long long, double> sectionweight;

		auto iter_stkweight = stkweight.cbegin();
		auto iter_stksection = stksection.cbegin();

		for (; iter_stksection != stksection.cend(); ++iter_stksection, ++iter_stkweight) {
			long long psection = *iter_stksection & sectionmask;
			auto i = sectionweight.find(psection);
			if (i == sectionweight.end()) {
				sectionweight.insert(std::make_pair(psection, *iter_stkweight));
			}
			else {
				i->second += *iter_stkweight;
			}
		}
		int cnt = static_cast<int>(sectionweight.size());
		section->resize(cnt);
		weight->resize(cnt);

		auto iter_result = sectionweight.cbegin();
		auto iter_rssection = section->begin();
		auto iter_rsweight = weight->begin();

		for (; iter_result != sectionweight.cend(); ++iter_result, ++iter_rssection, ++iter_rsweight) {
			*iter_rssection = iter_result->first;
			*iter_rsweight = iter_result->second;
		}
		return cnt;
	}

	void IndexInterface::writeFileReturnDaily(const char* file) const
	{
		std::fstream csvfile;
		csvfile.open(file, std::ios::out);
		int cnt = m_params->getDayCount();
		for (int i = 0; i < cnt; i++) {
			csvfile << m_params->getDateByIndex(i) << ',' << m_index->getDailyReturn()[i + 1] << ',' << m_index->getCumReturn()[i + 1] << std::endl;
		}
		csvfile.close();
	}
	void IndexInterface::writeFileReturnYearly(const char* file) const
	{
		std::fstream csvfile;
		csvfile.open(file, std::ios::out);

		double base = m_params->getIndexBase();
		int year = static_cast<int>(m_params->getDateByIndex(0)) / 10000;
		int cnt = m_params->getDayCount();
		for (int i = 0; i < cnt; i++) {
			if (i + 1 < cnt && m_params->getDateByIndex(i + 1) / 10000 == year) {
				continue;
			}
			csvfile << year << ',' << m_index->getCumReturn()[i + 1] / base - 1 << ',' << m_params->getDateByIndex(i) << ',' << m_index->getCumReturn()[i + 1] << std::endl;
			if (i + 1 < cnt) {
				year = static_cast<int>(m_params->getDateByIndex(i + 1)) / 10000;
				base = m_index->getCumReturn()[i + 1];
			}
		}

		csvfile.close();
	}

	void IndexInterface::writeFileWeightDay(const char* file, long long date) const
	{
		std::fstream csvfile;
		csvfile.open(file, std::ios::out);

		if (date == 0) {
			date = getPeriodEndDate();
		}
		std::vector<long long> stk;
		std::vector<double> weight;
		m_index->getOpenWeight(date, &stk, &weight);
		auto weight_iter = weight.cbegin();
		for (auto iter = stk.cbegin(); iter != stk.cend(); ++iter, ++weight_iter) {
			csvfile << *iter << ',' << *weight_iter << std::endl;
		}
		csvfile.close();
	}

	void IndexInterface::writeMapReturnDaily(std::map<int, double>* r) const
	{
		std::map<int, double> data;
		int cnt = m_params->getDayCount();
		for (int i = 0; i < cnt; i++) {
			data.try_emplace(static_cast<int>(m_params->getDateByIndex(i)), m_index->getCumReturn()[i + 1]);
		}
		r->swap(data);
	}

}
