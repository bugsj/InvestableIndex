#pragma once

namespace InvestableIndex {

	class DataColumn
	{
	private:
		HANDLE m_FileHandle = nullptr;
		HANDLE m_MapFileHandle = nullptr;
		const void* m_data = nullptr;
		long long m_size = 0;
		int m_itemsize = 0;
		long long m_itemcnt = 0;

	public:
		DataColumn() {}
		~DataColumn() { cleanup(); }

		long long init(const TCHAR* file, int itemsize);

		const void* getBuffer() const { return m_data; }
		const long long* getll() const { return reinterpret_cast<const long long*>(m_data); }
		const long long getll(long long index) const { return reinterpret_cast<const long long*>(m_data)[index]; }
		const double* getd() const { return reinterpret_cast<const double*>(m_data); }
		const double getd(long long index) const { return reinterpret_cast<const double*>(m_data)[index]; }

		long long getBufferSize() const { return m_size; }
		long long count() const { return m_itemcnt; }

	private:
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
	};

	template<typename T>
	class DataColumnType : public DataColumn
	{
	private:
		//DataColumn
	public:
		DataColumnType() { return; }
		int init(const TCHAR* file) { return init(file, sizeof(T)); }
		const T* get() const { return reinterpret_cast<const T*>(this->getBuffer()); }
	};

}
