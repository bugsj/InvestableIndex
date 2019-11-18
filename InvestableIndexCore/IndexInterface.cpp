#include "pch.h"
#include "IndexInterface.h"


HMODULE IndexInterface::m_module = NULL;

IndexInterface::IndexInterface()
{
	m_initevent = CreateEvent(NULL, TRUE, FALSE, NULL);
}

void IndexInterface::init()
{
	if (!m_atomic_init_flag.test_and_set()) {
		m_dataset.init();

		m_params_pool.emplace_back(IndexParameters());
		m_params = &m_params_pool.back();
		m_params->init(m_dataset);
		m_params->setSectionPool(m_dataset.getSectionPool());

		m_index_pool.emplace_back(IndexData());
		m_index = &m_index_pool.back();
		m_index->init(m_dataset);

		m_initflag = true;
		SetEvent(m_initevent);
	}
	else {
		WaitForSingleObject(m_initevent, 0);
	}
}

int IndexInterface::getOpenSectionWeight(long long date, vector<long long>* section, vector<double>* weight, int level) const
{
	vector<long long> stks;
	vector<double> stkweight;
	vector<long long> stksection;

	unsigned long long masks[] = { 0, 0xff, 0xffff, 0xffffff };
	unsigned long long sectionmask = masks[level];

	m_index->getOpenWeight(date, &stks, &stkweight);
	m_params->getStkSection(date, stks, &stksection);

	map<long long, double> sectionweight;

	auto iter_stkweight = stkweight.begin();
	auto iter_stksection = stksection.begin();

	for (; iter_stksection != stksection.end(); ++iter_stksection, ++iter_stkweight) {
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

	auto iter_result = sectionweight.begin();
	auto iter_rssection = section->begin();
	auto iter_rsweight = weight->begin();

	for (; iter_result != sectionweight.end(); ++iter_result, ++iter_rssection, ++iter_rsweight) {
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
	vector<long long> stk;
	vector<double> weight;
	m_index->getOpenWeight(date, &stk, &weight);
	auto weight_iter = weight.begin();
	for (auto iter = stk.begin(); iter != stk.end(); ++iter, ++weight_iter) {
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
