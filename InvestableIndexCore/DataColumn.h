#pragma once

namespace InvestableIndex {

	class DataColumn
	{
	private:
		HANDLE m_FileHandle = nullptr;
		HANDLE m_MapFileHandle = nullptr;
		const void* m_data = nullptr;
		long long m_size = 0;
		long long m_itemsize = 0;
		long long m_itemcnt = 0;

	private:
		long long init(const TCHAR* file, int itemsize);
		void cleanup() {
			if (m_MapFileHandle != nullptr) {
				::CloseHandle(m_MapFileHandle);
			}
			if (m_MapFileHandle != nullptr && m_FileHandle != INVALID_HANDLE_VALUE) {
				::CloseHandle(m_FileHandle);
			}
			m_data = nullptr;
			m_MapFileHandle = nullptr;
			m_FileHandle = nullptr;
		}

	public:
		DataColumn(const TCHAR* file, int itemsize) { init(file, itemsize); }
		DataColumn(const DataColumn& obj) = delete;
		DataColumn(DataColumn &&obj) noexcept : m_FileHandle(obj.m_FileHandle), m_MapFileHandle(obj.m_MapFileHandle),
			m_data(obj.m_data), m_size(obj.m_size), m_itemsize(obj.m_itemsize), m_itemcnt(obj.m_itemcnt)
		{
			obj.m_FileHandle = nullptr;
			obj.m_MapFileHandle = nullptr;
			obj.m_data = nullptr;
			obj.m_size = 0;
			obj.m_itemsize = 0;
			obj.m_itemcnt = 0;
		}
		~DataColumn() { cleanup(); }

		const void* getBuffer() const { return m_data; }
		const long long* getll() const { return reinterpret_cast<const long long*>(m_data); }
		const long long getll(long long index) const { return reinterpret_cast<const long long*>(m_data)[index]; }
		const double* getd() const { return reinterpret_cast<const double*>(m_data); }
		const double getd(long long index) const { return reinterpret_cast<const double*>(m_data)[index]; }

		long long getBufferSize() const { return m_size; }
		long long count() const { return m_itemcnt; }
	};

	template<typename T>
	class DataColumnType : public DataColumn
	{
	private:
		//DataColumn
	public:
		DataColumnType(const TCHAR* file) : DataColumn(file, sizeof(T)) {}
		const T* get() const { return reinterpret_cast<const T*>(this->getBuffer()); }
	};

}
