#pragma once
#include "SetFilePool.h"
class MultiSetFilePool :
	public SetFilePool
{
private:
	long long m_section = 0;
	unordered_map<int, vector<int> > m_sectionindex;
	unordered_map<int, map<int, int>> m_stkindex;
	vector<int>* m_currentsection = NULL;
public:
	long long init(const TCHAR* file);
	virtual long long getSnapshot(long long date, vector<long long>* list) const;

	long long setSection(long long section);
	long long getStkSection(long long date, const vector<long long>& stk, vector<long long>* section) const;
};
