#include "pch.h"
#include "DataColumn.h"


DataColumn::DataColumn()
{

}

int DataColumn::init(const TCHAR *file, int itemsize)
{
	m_FileHandle = CreateFile(file, GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
	if (m_FileHandle == INVALID_HANDLE_VALUE) {
		goto cleanup;
	}
	LARGE_INTEGER filesize;
	if (!GetFileSizeEx(m_FileHandle, &filesize)) {
		goto cleanup;
	}
	if (filesize.QuadPart == 0) {
		goto cleanup;
	}

	m_size = filesize.QuadPart;

	m_MapFileHandle = CreateFileMapping(m_FileHandle, NULL, PAGE_READONLY, filesize.u.HighPart, filesize.u.LowPart, NULL);
	if (m_MapFileHandle == NULL) {
		goto cleanup;
	}

	m_data = MapViewOfFile(m_MapFileHandle, FILE_MAP_READ, 0, 0, 0);

	m_itemsize = itemsize;
	if (itemsize != 0) {
		m_itemcnt = m_size / itemsize;
	}

	return static_cast<int>(m_itemcnt);

cleanup:

	m_data = NULL;

	if (m_MapFileHandle != NULL) {
		CloseHandle(m_MapFileHandle);
	}

	if (m_FileHandle != INVALID_HANDLE_VALUE) {
		CloseHandle(m_FileHandle);
	}
	return 0;
}



DataColumn::~DataColumn()
{
	if (m_MapFileHandle != NULL) {
		CloseHandle(m_MapFileHandle);
	}

	if (m_FileHandle != INVALID_HANDLE_VALUE) {
		CloseHandle(m_FileHandle);
	}
}
