#include "pch.h"
#include "DataTable.h"


vector<TCHAR> DataTable::s_defaultnet_path = std::vector<TCHAR>(DEFAULT_NET_PATH, DEFAULT_NET_PATH + wcslen(DEFAULT_NET_PATH) + 1);
const vector<TCHAR>* DataTable::s_path = &DataTable::s_defaultnet_path;

static inline long long mergecd(long long code, long long date)
{
	return (code << 32) + date;
}

//static int mergecdint(long long code, long long date)
//{
//	int sdate = static_cast<int>(date) - 19990000;
//	int scode = static_cast<int>(code);
//	return (scode % 70000) * 16384 + (sdate / 10000) * 512 + sdate % 10000 / 100 * 32 + sdate % 100;
//}

DataTable::DataTable()
{
}

DataTable::~DataTable()
{
}


//int DataTable::init(const TCHAR *name, int colcnt, const TCHAR **colname, const int *colitemsize)
//{
//	TCHAR fullname[MAX_PATH];
//	for (int i = 0; i < colcnt; ++i) {
//		_sntprintf_s(fullname, MAX_PATH, MAX_PATH, s_pathfmt, name, colname[i]);
//		m_columns.emplace_back(std::make_unique<DataColumn>());
//		m_columns.back()->init(fullname, colitemsize[i]);
//	}
//	return colcnt;
//}

int DataTable::init(const TCHAR* name)
{
	if (name == nullptr) {
		return 0;
	}

	vector<WCHAR> path = *s_path;
	const WCHAR* ext = COL_DESC_FILE_EXT;
	size_t pathsize = wcslen(path.data());
	size_t namesize = wcslen(name);
	size_t extsize = wcslen(ext);
	vector<WCHAR> tabfile(pathsize + 1 + namesize + extsize + 1);
	CopyMemory(tabfile.data(), path.data(), sizeof(*path.data()) * pathsize);
	tabfile[pathsize] = L'\\';
	CopyMemory(tabfile.data() + pathsize + 1, name, sizeof(*name) * namesize);
	CopyMemory(tabfile.data() + pathsize + 1 + namesize, ext, sizeof(*ext) * extsize);
	tabfile[pathsize + 1 + namesize + extsize] = L'\0';

	FILE* fp;
	if ((fp = _wfsopen(tabfile.data(), L"r", _SH_DENYNO)) == NULL) {
		return 0;
	}
		
	const int bufsize = 256;
	char buf[bufsize];

	std::vector<std::vector<WCHAR> > colfile;
	std::vector<int> colwidth;
	while (fgets(buf, bufsize, fp)) {
		char* ptr = buf;
		while(*ptr != ',' && *ptr != '\0' && *ptr != '\n' && ptr - buf < bufsize) {
			++ptr;
		}
		size_t colname_size = ptr - buf;
		colfile.emplace_back(vector<WCHAR>(colname_size + 1));
		int wchar_size = MultiByteToWideChar(CP_UTF8, 0, buf, static_cast<int>(colname_size), colfile.back().data(), static_cast<int>(colname_size));
		DWORD err = ::GetLastError();
		colfile.back()[wchar_size] = L'\0';

		if (*ptr == '\0' || *ptr == '\n') {
			colwidth.push_back(8);
		}
		else if (*ptr == ',') {
			int n = atoi(ptr + 1);
			if (n > 0) {
				colwidth.push_back(n);
			}
			else {
				colwidth.push_back(8);
			}
		}
		else {
			colwidth.push_back(8);
		}
	}
	fclose(fp);

	TCHAR fullname[MAX_PATH];
	vector<WCHAR> pathfmt;
	getPathFormat(&pathfmt);
	for (unsigned int i = 0; i < colfile.size(); ++i) {
		_sntprintf_s(fullname, MAX_PATH, MAX_PATH, pathfmt.data(), name, colfile[i].data());
		m_columns.emplace_back(std::make_unique<DataColumn>());
		m_columns.back()->init(fullname, colwidth[i]);
	}
	return static_cast<int>(colfile.size());
}

void DataTable::createIndexDate()
{
	const long long *refcode = m_columns[STKCODEINDEX]->getll();
	const long long *refcodeend = m_columns[STKCODEINDEX]->getll() + m_columns[STKCODEINDEX]->count();
	const long long *refdate = m_columns[STKDATEINDEX]->getll();
	int index = 0;

	m_index_date.reserve(245 * 12);
	m_index_date.rehash(245 * 12);

	while (refcode != refcodeend) {
		unordered_map<int, unordered_map<int, int> >::iterator iter;
		iter = m_index_date.find(static_cast<int>(*refdate));
		if (iter == m_index_date.end()) {
			iter = m_index_date.emplace(static_cast<int>(*refdate), unordered_map<int, int>()).first;
			iter->second.reserve(4000);
			iter->second.rehash(4000);
		}
		iter->second.emplace(static_cast<int>(*refcode), index);
		++refcode;
		++refdate;
		++index;
	}
}


void DataTable::createIndexCode()
{
	const long long *refcode = m_columns[STKCODEINDEX]->getll();
	const long long *refcodeend = m_columns[STKCODEINDEX]->getll() + m_columns[STKCODEINDEX]->count();
	const long long *refdate = m_columns[STKDATEINDEX]->getll();
	int index = 0;

	while (refcode != refcodeend) {
		unordered_map<int, map<int, int> >::iterator iter;
		iter = m_index_code.find(static_cast<int>(*refcode));
		if (iter == m_index_code.end()) {
			iter = m_index_code.emplace(static_cast<int>(*refcode), map<int, int>()).first;
		}
		iter->second.emplace(static_cast<int>(*refdate), index);
		++refcode;
		++refdate;
		++index;
	}
}

long long DataTable::getLastDate(long long code, long long date) const
{
	int scode, sdate;
	scode = static_cast<int>(code);
	sdate = static_cast<int>(date);
	const map<int, int>& datelist = m_index_code.at(scode);
	map<int, int>::const_iterator iter;
	iter = datelist.upper_bound(sdate);
	if (iter == datelist.begin()) {
		return iter->second;
	}
	--iter;

	return iter->first;
}

long long DataTable::getLastDateIndex(long long code, long long date) const
{
	int scode, sdate;
	scode = static_cast<int>(code);
	sdate = static_cast<int>(date);
	const map<int, int>& datelist = m_index_code.at(scode);
	map<int, int>::const_iterator iter;
	iter = datelist.upper_bound(sdate);
	if (iter == datelist.begin()) {
		return iter->second;
	}
	--iter;

	return  iter->second;
}
