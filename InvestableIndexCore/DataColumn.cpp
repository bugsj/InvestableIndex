#include "pch.h"

#include "DataColumn.h"
#include "DebugOutput.h"

namespace InvestableIndex {

	long long DataColumn::init(const TCHAR* file, int itemsize)
	{
		Debug::WriteLog(file);

		m_FileHandle = ::CreateFile(file, GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
		if (m_FileHandle == INVALID_HANDLE_VALUE) {
			cleanup();
			return 0;
		}

		::LARGE_INTEGER filesize;
		if (!::GetFileSizeEx(m_FileHandle, &filesize) || filesize.QuadPart == 0) {
			cleanup();
			return 0;
		}

		m_size = filesize.QuadPart;
		m_MapFileHandle = ::CreateFileMapping(m_FileHandle, NULL, PAGE_READONLY, filesize.u.HighPart, filesize.u.LowPart, NULL);
		if (m_MapFileHandle == NULL) {
			cleanup();
			return 0;
		}

		m_data = ::MapViewOfFile(m_MapFileHandle, FILE_MAP_READ, 0, 0, 0);
		m_itemsize = itemsize;
		if (itemsize != 0) {
			m_itemcnt = m_size / itemsize;
		}

		return m_itemcnt;
	}

}
