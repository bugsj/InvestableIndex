#pragma once

#define ERR_NOPOOLS (-1)

namespace InvestableIndex {

	class StkPool
	{
	public:
		virtual long long getSnapshot(long long date, std::vector<long long>* list) const = 0;
		virtual ~StkPool() {}
	};

}
