#include "pch.h"

#include "IndexInterface.h"

namespace InvestableIndex {

	HMODULE IndexInterface::m_module = NULL;

	void IndexInterface::init()
	{
		std::lock_guard<std::mutex> lck(m_init_mtx);

		if (!m_initflag) {
			if (IndexInterface::m_module != NULL) {
				TCHAR dllpath[MAX_PATH];
				long long size = ::GetModuleFileName(IndexInterface::m_module, dllpath, MAX_PATH);
				if (size > 0 && size < MAX_PATH) {
					long long cut = -1;
					for (long long i = 0; i < size; ++i) {
						if (dllpath[i] == L'\\') {
							cut = i;
						}
					}
					if (cut > 0) {
						dllpath[cut + 1] = L'\0';
						std::vector<TCHAR> localpath;
						m_dataset.setLocal(Tools::appendStr(&localpath, { dllpath, DATAFILE_SUBDIR }));
						DataTable::useLocal();
					}
				}
			}

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

	long long IndexInterface::getOpenSectionWeight(long long date, std::vector<long long>* section, std::vector<double>* weight, long long level) const
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

		long long cnt = sectionweight.size();
		section->resize(cnt);
		weight->resize(cnt);
		auto iter_rssection = section->begin();
		auto iter_rsweight = weight->begin();
		for (auto iter_result :sectionweight) {
			*iter_rssection++ = iter_result.first;
			*iter_rsweight++ = iter_result.second;
		}
		return cnt;
	}

	void IndexInterface::writeFileReturnDaily(const char* file) const
	{
		std::fstream csvfile;
		csvfile.open(file, std::ios::out);
		long long cnt = m_params->getDayCount();
		for (long long i = 0; i < cnt; i++) {
			csvfile << m_params->getDateByIndex(i) << ',' << m_index->getDailyReturn()[i + 1] << ',' << m_index->getCumReturn()[i + 1] << std::endl;
		}
		csvfile.close();
	}
	void IndexInterface::writeFileReturnYearly(const char* file) const
	{
		std::fstream csvfile;
		csvfile.open(file, std::ios::out);

		double base = static_cast<double>(m_params->getIndexBase());
		long long year = m_params->getDateByIndex(0) / 10000;
		long long cnt = m_params->getDayCount();
		for (long long i = 0; i < cnt; i++) {
			if (i + 1 < cnt && m_params->getDateByIndex(i + 1) / 10000 == year) {
				continue;
			}
			csvfile << year << ',' << m_index->getCumReturn()[i + 1] / base - 1 << ',' << m_params->getDateByIndex(i) << ',' << m_index->getCumReturn()[i + 1] << std::endl;
			if (i + 1 < cnt) {
				year = m_params->getDateByIndex(i + 1) / 10000;
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
		for (auto iter :stk) {
			csvfile << iter << ',' << *weight_iter++ << std::endl;
		}
		csvfile.close();
	}

	void IndexInterface::writeMapReturnDaily(std::map<int, double>* r) const
	{
		std::map<int, double> data;
		long long cnt = m_params->getDayCount();
		for (long long i = 0; i < cnt; i++) {
			data.try_emplace(CHECK_DATE_MAX(m_params->getDateByIndex(i)), m_index->getCumReturn()[i + 1]);
		}
		r->swap(data);
	}

}
