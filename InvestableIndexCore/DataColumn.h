#pragma once
class DataColumn
{
private:
	HANDLE m_FileHandle = NULL;
	HANDLE m_MapFileHandle = NULL;
	const void *m_data = NULL;
	long long m_size = 0;
	int m_itemsize = 0;
	long long m_itemcnt = 0;

public:
	DataColumn();
	~DataColumn();

	int init(const TCHAR *file, int itemsize);

	const void *getBuffer() const { return m_data; }
	const long long *getll() const { return reinterpret_cast<const long long *>(m_data); }
	const long long getll(long long index) const { return reinterpret_cast<const long long *>(m_data)[index]; }

	long long getBufferSize() const { return m_size; }
	long long count() const { return m_itemcnt; }
};

template<typename T>
class DataColumnType : public DataColumn
{
private:
	//DataColumn
public:
	DataColumnType() { return; }
	int init(const TCHAR *file) { return init(file, sizeof(T)); }
	const T *get() const { return reinterpret_cast<const T *>(this->getBuffer()); }
};

