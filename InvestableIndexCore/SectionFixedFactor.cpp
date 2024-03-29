#include "pch.h"

#include "SectionFixedFactor.h"

namespace InvestableIndex {

	long long SectionFixedFactor::setSectionWeight(const std::vector<int>& sections, const std::vector<double>& weights)
	{
		m_sectionweight.clear();
		auto iter_section = sections.begin();
		auto iter_weight = weights.begin();

		for (; iter_section != sections.end(); ++iter_section, ++iter_weight) {
			m_sectionweight.insert(std::make_pair(*iter_section, *iter_weight));
		}

		return m_sectionweight.size();
	}

	struct SectionData {
		std::vector<int> index;
		double weight;
		double target;
		double factor;
	};

	long long SectionFixedFactor::getFactor(long long date, const std::vector<long long>& stk, std::vector<double>* factor) const
	{
		/*
		std::vector<long long> idxcon;
		getIndexParameters()->getPool()->getSnapshot(date, &idxcon);
		size_t size = idxcon.size();

		factor->resize(size);
		if (size == 0) {
			return size;
		}

		std::vector<long long> stk_section;
		m_section->getStkSection(date, stk, &stk_section);
		std::unordered_map<int, SectionData> section_summary;
		std::unordered_map<int, SectionData>::iterator iter_summary;

		vector<std::unordered_map<int, SectionData>::const_iterator> stk_summary(size);
		vector<double> mv(size);
		vector<size_t> orderidx(size);

		std::iota(orderidx.begin(), orderidx.end(), 0);
		std::fill(factor->begin(), factor->end(), 1.0);

		double totalmv = 0;
		auto iter_mv = mv.begin();
		auto iter_section = stk_section.begin();
		auto iter_stksummary = stk_summary.begin();
		int stkindex = 0;
		const unordered_map<int, int>& stksearcher = getDataSet()->getStkIndexOfDay(date);
		for (auto iter = idxcon.begin(); iter != idxcon.end(); ++iter, ++iter_mv, ++iter_section, ++stkindex, ++iter_stksummary) {
			long long index;

			auto stkrs = stksearcher.find(static_cast<const int>(*iter));
			if (stkrs == stksearcher.end()) {
				*iter_mv = 0;
				continue;
			}
			index = stkrs->second;
			*iter_mv = getDataSet()->close(index) * getDataSet()->freeshare(*iter, date);
			totalmv += *iter_mv;

			iter_summary = section_summary.find(*iter_section);
			if (iter_summary == section_summary.end()) {
				auto item = section_summary.insert(std::make_pair(*iter_section, SectionData()));
				item.first->second.index.push_back(stkindex);
				item.first->second.weight = *iter_mv;
				*iter_stksummary = item.first;
			}
			else {
				iter_summary->second.index.push_back(stkindex);
				iter_summary->second.weight += *iter_mv;
				*iter_stksummary = iter_summary;
			}
		}

		if (totalmv == 0) {
			return 0;
		}

		double total_target = 0;
		vector<SectionData*> abovetarget, belowtarget, notarget;
		double belowweight = 0, otherweight = 0, shortage = 0;
		for (iter_summary = section_summary.begin(); iter_summary != section_summary.end(); ++iter_summary) {
			iter_summary->second.weight /= totalmv;
			auto iter_weight = m_sectionweight.find(iter_summary->first);
			if (iter_weight == m_sectionweight.end()) {
				iter_summary->second.target = 0;
				notarget.push_back(&iter_summary->second);
				otherweight += iter_summary->second.weight;
				continue;
			}
			total_target += iter_weight->second;
			iter_summary->second.target = iter_weight->second;
			iter_summary->second.factor = iter_summary->second.target / iter_summary->second.weight;
			if (iter_summary->second.target > iter_summary->second.weight) {
				belowtarget.push_back(&iter_summary->second);
				belowweight += iter_summary->second.weight;
				shortage += (iter_summary->second.target - iter_summary->second.weight);
			}
			else {
				abovetarget.push_back(&iter_summary->second);
				otherweight += iter_summary->second.weight;
			}
		}
		if (total_target < 0.001 || belowtarget.empty()) {
			return size;
		}
		std::sort(abovetarget.begin(), abovetarget.end(), [](const SectionData* i1, const SectionData* i2) { return i1->factor > i2->factor; });
		double adjfactor;
		while (!abovetarget.empty()) {
			adjfactor = (otherweight - shortage) / otherweight;
			if (abovetarget.back()->factor > adjfactor) {
				shortage += (abovetarget.back()->target - abovetarget.back()->weight);
				belowweight += abovetarget.back()->weight;
				otherweight -= abovetarget.back()->weight;
				belowtarget.push_back(abovetarget.back());
				abovetarget.pop_back();
			}
		}
		std::for_each(belowtarget.begin(), belowtarget.end(), [](SectionData* item) { item->factor = item->target / item->weight; });
		std::for_each(abovetarget.begin(), abovetarget.end(), [adjfactor](SectionData* item) { item->factor = adjfactor; });
		std::for_each(belowtarget.begin(), belowtarget.end(), [adjfactor](SectionData* item) { item->factor = adjfactor; });

		vector<double> mv_rate(size);
		auto rateiter = mv_rate.begin();
		auto summiter = stk_summary.begin();
		for (iter_mv = mv.begin(); iter_mv != mv.end(); ++iter_mv, ++rateiter, ++summiter) {
			*rateiter = (*iter_mv) * (*summiter)->second.factor / totalmv;
		}

		std::sort(orderidx.begin(), orderidx.end(), [&mv](size_t i1, size_t i2) { return mv[i1] > mv[i2]; });

		size_t idx = 0;
		double remain = 0;
		double adj;
		while (idx < size && mv_rate[orderidx[idx]] > getTopLimit()) {
			remain = 0;
			for (auto iter = stk_summary[idx]->second.index.begin(); iter != stk_summary[idx]->second.index.end(); ++iter) {

			}
			adj = (remain - getTopLimit()) / (remain - mv_rate[orderidx[idx]]);
			mv_rate[orderidx[idx]] = getTopLimit();
			remain -= getTopLimit();
			++idx;
			for (auto iter = orderidx.begin() + idx; iter != orderidx.end(); ++iter) {
				mv_rate[*iter] *= adj;
			}
		}

		// 比率调整完，转化为factor;
		if (idx == size) {
			--idx;
		}
		double ratio_mv = 0, ratio_mvrate = 0;
		for (auto iter = orderidx.begin() + idx; iter != orderidx.end(); ++iter) {
			ratio_mv += mv[*iter];
			ratio_mvrate += mv_rate[*iter];
		}
		double topvalue = ratio_mv / ratio_mvrate * getTopLimit();
		for (auto iter = orderidx.begin(); iter != orderidx.begin() + idx; ++iter) {
			factor->at(*iter) = topvalue / mv[*iter];
		}

		return size;
		*/
		return 0;
	}

}
