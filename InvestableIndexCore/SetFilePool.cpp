#include "pch.h"

#include "SetFilePool.h"

namespace InvestableIndex {

	long long SetFilePool::getSnapshot(long long date, std::vector<long long>* list) const
	{
		const long long* refcode = m_table.col(STKCODEINDEX).getll();
		const long long* refcodeend = m_table.col(STKCODEINDEX).getll() + m_table.col(STKCODEINDEX).count();
		const long long* refentrydate = m_table.col(STKENTRYDATEINDEX).getll();
		const long long* refexitdate = m_table.col(STKEXITDATEINDEX).getll();

		while (refcode != refcodeend) {
			if (*refentrydate <= date && *refexitdate >= date) {
				list->push_back(*refcode);
			}
			++refcode;
			++refentrydate;
			++refexitdate;
		}

		return list->size();
	}

}
