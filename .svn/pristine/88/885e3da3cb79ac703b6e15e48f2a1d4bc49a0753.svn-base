#include "pch.h"
#include "InterfaceC.h"
#include "IndexInterface.h"
#include "CUI.h"


extern "C" INVESTABLEINDEXCORE_API int IITestInterface(int i);

__declspec(dllexport) int IITestInterface(int i)
{
	return i + 1;
}

void DLLCEntry(HMODULE hModule, DWORD ul_reason_for_call)
{
	switch (ul_reason_for_call)
	{
	case DLL_PROCESS_ATTACH:
		IndexInterface::setModuleHandle(hModule);
		break;
	case DLL_THREAD_ATTACH:
		break;
	case DLL_THREAD_DETACH:
		break;
	case DLL_PROCESS_DETACH:
		break;
	}
	return;
}

int checkTradeDate(int *date) 
{
	int refdate = static_cast<int>(IndexInterface::get()->getLastTradeDate(*date));
	
	int rs = (refdate <= *date ? 0 : 1);

	*date = refdate;
	return rs;
}

INVESTABLEINDEXCORE_API	int IIInit()
{
	IndexInterface::get();
	return 0; // indexobj
}
INVESTABLEINDEXCORE_API	int IIRelease()
{
	return 0;
}

INVESTABLEINDEXCORE_API int IISetIndexBase(int indexobj, int base)
{
	return IndexInterface::get()->setIndexBase(base);
}

INVESTABLEINDEXCORE_API int IISimulate(int indexobj)
{
	IndexInterface::get()->simulate();
	return 0;
}

INVESTABLEINDEXCORE_API int IIGetIndexBase(int indexobj)
{
	return IndexInterface::get()->getIndexBase();
}

INVESTABLEINDEXCORE_API int IISetStartDate(int indexobj, int date)
{
	return static_cast<int>(IndexInterface::get()->setPeriodStartDate(date));
}

INVESTABLEINDEXCORE_API int IISetEndDate(int indexobj, int date)
{
	return static_cast<int>(IndexInterface::get()->setPeriodEndDate(date));
}

INVESTABLEINDEXCORE_API int IISetFixDateFlag(int indexobj, int flag)
{
	IndexInterface::get()->setFixedDate(flag);
	return flag;
}

INVESTABLEINDEXCORE_API int IISetTopStkWeight(int indexobj, int weight)
{
	IndexInterface::get()->setTopStkWeight(weight);
	return weight;
}

INVESTABLEINDEXCORE_API int IISetTopStkCount(int indexobj, int cnt)
{
	IndexInterface::get()->setTopStkCount(cnt);
	return cnt;
}

INVESTABLEINDEXCORE_API int IISetIndexBasePool(int indexobj, int base)
{
	return IndexInterface::get()->setIndexBasePool(base);
}

INVESTABLEINDEXCORE_API int IIAppendCSVPool(int indexobj, const char* file)
{
	return IndexInterface::get()->appendCSVPool(file);
}

INVESTABLEINDEXCORE_API int IIAppendSimplePool(int indexobj, int size, const int *buf)
{
	return IndexInterface::get()->appendSimplePool(size, buf);
}

INVESTABLEINDEXCORE_API int IIAppendSectionPool(int indexobj, int section)
{
	return IndexInterface::get()->appendSectionPool(section);
}

INVESTABLEINDEXCORE_API int IIRemovePools(int indexobj)
{
	IndexInterface::get()->removePools();
	return 0;
}

INVESTABLEINDEXCORE_API int IIGetWeightCount(int indexobj, int date)
{
	if (checkTradeDate(&date)) {
		return 0;
	}
	std::vector<long long> stk;
	std::vector<double> weight;
	IndexInterface::get()->getOpenWeight(date, &stk, &weight);
	return static_cast<int>(stk.size());
}

INVESTABLEINDEXCORE_API int IIGetWeight(int indexobj, int date, int* stks, double* weights)
{
	if (checkTradeDate(&date)) {
		return 0;
	}
	std::vector<long long> stk;
	std::vector<double> weight;
	IndexInterface::get()->getOpenWeight(date, &stk, &weight);
	auto stk_iter = stk.begin();
	auto weight_iter = weight.begin();
	int* stk_ptr = stks;
	double* weight_ptr = weights;
	for (; stk_iter != stk.end(); ++stk_iter, ++weight_iter, ++stk_ptr, ++weight_ptr) {
		*stk_ptr = static_cast<int>(*stk_iter);
		*weight_ptr = *weight_iter;
	}
	return static_cast<int>(stk.size());
}

INVESTABLEINDEXCORE_API int IIGetSecWeightCount(int indexobj, int date, int level)
{
	if (checkTradeDate(&date)) {
		return 0;
	}
	std::vector<long long> sctn;
	std::vector<double> weight;
	IndexInterface::get()->getOpenSectionWeight(date, &sctn, &weight, level);
	return static_cast<int>(sctn.size());
}

INVESTABLEINDEXCORE_API int IIGetSecWeight(int indexobj, int date, int* section, double* weights, int level)
{
	if (checkTradeDate(&date)) {
		return 0;
	}
	std::vector<long long> sctn;
	std::vector<double> weight;
	IndexInterface::get()->getOpenSectionWeight(date, &sctn, &weight, level);
	auto stk_iter = sctn.begin();
	auto weight_iter = weight.begin();
	int* stk_ptr = section;
	double* weight_ptr = weights;
	for (; stk_iter != sctn.end(); ++stk_iter, ++weight_iter, ++stk_ptr, ++weight_ptr) {
		*stk_ptr = static_cast<int>(*stk_iter);
		*weight_ptr = *weight_iter;
	}
	return static_cast<int>(sctn.size());
}

INVESTABLEINDEXCORE_API int IIGetDayCount(int indexobj)
{
	return IndexInterface::get()->getDayCount();
}

INVESTABLEINDEXCORE_API int IIGetDates(int indexobj, int* dates)
{
	for (long long i = IndexInterface::get()->getDayCount() - 1LL; i >= 0; --i) {
		dates[i] = static_cast<int>(IndexInterface::get()->getDateByIndex(i));
	}
	return IndexInterface::get()->getDayCount();
}

INVESTABLEINDEXCORE_API int IIGetDailyReturn(int indexobj, double* r)
{
	if (r != nullptr) {
		std::copy(IndexInterface::get()->getDailyReturn().begin(), IndexInterface::get()->getDailyReturn().end(), r);
	}
	return static_cast<int>(IndexInterface::get()->getDailyReturn().size());
}

INVESTABLEINDEXCORE_API int IIGetCumReturn(int indexobj, double* r)
{
	if (r != nullptr) {
		std::copy(IndexInterface::get()->getCumReturn().begin(), IndexInterface::get()->getCumReturn().end(), r);
	}
	return static_cast<int>(IndexInterface::get()->getCumReturn().size());
}

INVESTABLEINDEXCORE_API int IIGetDateByIndex(int indexobj, int index)
{
	return static_cast<int>(IndexInterface::get()->getDateByIndex(index));
}

INVESTABLEINDEXCORE_API int IIWriteReturnDaily(int indexobj, const char* file)
{
	IndexInterface::get()->writeFileReturnDaily(file);
	return 0;
}

INVESTABLEINDEXCORE_API int IIWriteReturnYearly(int indexobj, const char* file)
{
	IndexInterface::get()->writeFileReturnYearly(file);
	return 0;
}

INVESTABLEINDEXCORE_API int IIWriteWeightDay(int indexobj, const char* file, int date)
{
	if (checkTradeDate(&date)) {
		return 0;
	}
	IndexInterface::get()->writeFileWeightDay(file, date);
	return 0;
}

INVESTABLEINDEXCORE_API int ProcEntry(HWND hwnd, HINSTANCE hinst, LPTSTR cmdline, int cmdshow)
{
	return InvestableIndexCUI();
}
