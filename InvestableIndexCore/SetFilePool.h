#pragma once
#include "StkPool.h"
#include "DataTable.h"

class SetFilePool :
	public StkPool
{
private:
	DataTable m_table;

public:
	SetFilePool();
	~SetFilePool();

	const DataTable& getDataTable() const { return m_table; }
	long long init(const TCHAR *file) { return m_table.init(file); }

	virtual long long getSnapshot(long long date, vector<long long> *list) const;
};

