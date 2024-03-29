#include "pch.h"

#include "DataSet.h"
#include "PeriodEvents.h"

namespace InvestableIndex {

	long long PeriodEvents::getLastEventDate(long long date) const
	{
		if (m_EventDates.empty()) {
			return date;
		}

		auto ref = m_EventDates.upper_bound(date);
		if (ref != m_EventDates.begin()) {
			--ref;
		}
		return *ref;
	}

	void PeriodEvents::setHalfYearEvent()
	{
		long long ybegin, yend, d;
		ybegin = m_DataSet->getCalendarData().getll(0) / 10000;
		yend = m_DataSet->getCalendarData().getll(m_DataSet->getCalendarData().count() - 1) / 10000;

		cleanEvents();

		for (long long y = ybegin; y <= yend; ++y) {
			d = m_DataSet->getLastTradeDateIndex(y * 10000 + 630);
			addEvent(m_DataSet->getCalendarData().getll(d));
			d = m_DataSet->getLastTradeDateIndex(y * 10000 + 1231);
			addEvent(m_DataSet->getCalendarData().getll(d));
		}
	}

}
