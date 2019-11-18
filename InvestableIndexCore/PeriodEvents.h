#pragma once

class DataSet;

class PeriodEvents
{
private:
	const DataSet* m_DataSet;
	std::set<long long> m_EventDates;

public:
	void init(const DataSet& dataset) { m_DataSet = &dataset; }
	void addEvent(long long date) { m_EventDates.insert(date); }
	void addEvents(const std::vector<long long>& events) { m_EventDates.insert(events.begin(), events.end()); }
	void addEvents(const long long* events, size_t cnt) { m_EventDates.insert(events, events + cnt); }
	void setHalfYearEvent();
	int setEvent(int flag) { setHalfYearEvent(); return flag; }
	void cleanEvents() { m_EventDates.clear(); }
	long long getLastEventDate(long long date) const;
};
