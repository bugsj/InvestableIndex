#include "pch.h"
#include "SetFilePool.h"

#define STKCODEINDEX 0
#define STKDATEINDEX 1
#define STKENTRYDATEINDEX 1
#define STKEXITDATEINDEX 2
#define STKSECTION 3

SetFilePool::SetFilePool()
{
}


SetFilePool::~SetFilePool()
{
}

long long SetFilePool::getSnapshot(long long date, vector<long long> *list) const
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
