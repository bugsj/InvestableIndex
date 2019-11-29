#pragma once

#include "StkPool.h"
#include "DataSet.h"

namespace InvestableIndex {

	class IndexParameters;

	class SizeTopPool :
		public StkPool
	{
	private:
		const StkPool* m_Pool = nullptr;
		const DataSet* m_DataSet = nullptr;
		const IndexParameters* m_Params = nullptr;
		WeightType m_WeightType = WeightType::FREESHARE;
		long long m_TopCount = 50;
	public:
		SizeTopPool() {}
		~SizeTopPool() {}

		void init(const StkPool& base, const DataSet& data, const IndexParameters& params) { m_Pool = &base; m_DataSet = &data; m_Params = &params; }
		StkPool& setTopCount(long long topcnt) { m_TopCount = topcnt; return *this; }
		StkPool& setPool(StkPool& pool, WeightType wt) { m_Pool = &pool; m_WeightType = wt; return *this; }

		virtual long long getSnapshot(long long date, std::vector<long long>* list) const;
	};

}
