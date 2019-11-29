#include "pch.h"

#include "IndexInterface.h"
#include "CUI.h"

#include "InterfaceC.h"


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
		InvestableIndex::IndexInterface::setModuleHandle(hModule);
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

static InvestableIndex::IndexInterface* InterfaceC_IIIndex = nullptr;
#define CHECK_INDEX_PTR do { if (InterfaceC_IIIndex == nullptr) { return -1; } } while (0)

int checkTradeDate(int *date) 
{
	int refdate = CHECK_DATE_MAX(InterfaceC_IIIndex->getLastTradeDate(*date));
	
	int rs = (refdate <= *date ? 0 : 1);

	*date = refdate;
	return rs;
}


INVESTABLEINDEXCORE_API	int IIInit()
{
	try {
		InterfaceC_IIIndex = InvestableIndex::IndexInterface::get();
	}
	catch (std::runtime_error e) {
		InterfaceC_IIIndex = nullptr;
	}
	CHECK_INDEX_PTR;
	return 0; // indexobj
}
INVESTABLEINDEXCORE_API	int IIRelease()
{
	return 0;
}

INVESTABLEINDEXCORE_API int IISetIndexBase(int indexobj, int base)
{
	CHECK_INDEX_PTR;
	return CHECK_VALUE_MAXINT32(InterfaceC_IIIndex->setIndexBase(base));
}

INVESTABLEINDEXCORE_API int IISimulate(int indexobj)
{
	CHECK_INDEX_PTR;
	InterfaceC_IIIndex->simulate();
	return 0;
}

INVESTABLEINDEXCORE_API int IIGetIndexBase(int indexobj)
{
	CHECK_INDEX_PTR;
	return CHECK_VALUE_MAXINT32(InterfaceC_IIIndex->getIndexBase());
}

INVESTABLEINDEXCORE_API int IISetStartDate(int indexobj, int date)
{
	CHECK_INDEX_PTR;
	return CHECK_DATE_MAX(InterfaceC_IIIndex->setPeriodStartDate(date));
}

INVESTABLEINDEXCORE_API int IISetEndDate(int indexobj, int date)
{
	CHECK_INDEX_PTR;
	return CHECK_DATE_MAX(InterfaceC_IIIndex->setPeriodEndDate(date));
}

INVESTABLEINDEXCORE_API int IISetFixDateFlag(int indexobj, int flag)
{
	CHECK_INDEX_PTR;
	InterfaceC_IIIndex->setFixedDate(flag);
	return flag;
}

INVESTABLEINDEXCORE_API int IISetTopStkWeight(int indexobj, int weight)
{
	CHECK_INDEX_PTR;
	InterfaceC_IIIndex->setTopStkWeight(weight);
	return weight;
}

INVESTABLEINDEXCORE_API int IISetTopStkCount(int indexobj, int cnt)
{
	CHECK_INDEX_PTR;
	InterfaceC_IIIndex->setTopStkCount(cnt);
	return cnt;
}

INVESTABLEINDEXCORE_API int IISetIndexBasePool(int indexobj, int base)
{
	CHECK_INDEX_PTR;
	return InterfaceC_IIIndex->setIndexBasePool(base);
}

INVESTABLEINDEXCORE_API int IIAppendCSVPool(int indexobj, const char* file)
{
	CHECK_INDEX_PTR;
	return CHECK_SIZE_MAXINT32(InterfaceC_IIIndex->appendCSVPool(file));
}

INVESTABLEINDEXCORE_API int IIAppendSimplePool(int indexobj, int size, const int *buf)
{
	CHECK_INDEX_PTR;
	return CHECK_SIZE_MAXINT32(InterfaceC_IIIndex->appendSimplePool(size, buf));
}

INVESTABLEINDEXCORE_API int IIAppendSectionPool(int indexobj, int section)
{
	CHECK_INDEX_PTR;
	return CHECK_SIZE_MAXINT32(InterfaceC_IIIndex->appendSectionPool(section));
}

INVESTABLEINDEXCORE_API int IIRemovePools(int indexobj)
{
	CHECK_INDEX_PTR;
	InterfaceC_IIIndex->removePools();
	return 0;
}

INVESTABLEINDEXCORE_API int IIGetWeightCount(int indexobj, int date)
{
	CHECK_INDEX_PTR;
	if (checkTradeDate(&date)) {
		return 0;
	}
	std::vector<long long> stk;
	std::vector<double> weight;
	InterfaceC_IIIndex->getOpenWeight(date, &stk, &weight);
	return CHECK_SIZE_MAXINT32(stk.size());
}

INVESTABLEINDEXCORE_API int IIGetWeight(int indexobj, int date, int* stks, double* weights)
{
	CHECK_INDEX_PTR;
	if (checkTradeDate(&date)) {
		return 0;
	}
	std::vector<long long> stk;
	std::vector<double> weight;
	InterfaceC_IIIndex->getOpenWeight(date, &stk, &weight);
	auto stk_iter = stk.begin();
	auto weight_iter = weight.begin();
	int* stk_ptr = stks;
	double* weight_ptr = weights;
	for (; stk_iter != stk.end(); ++stk_iter, ++weight_iter, ++stk_ptr, ++weight_ptr) {
		*stk_ptr = CHECK_CODE_MAX(*stk_iter);
		*weight_ptr = *weight_iter;
	}
	return CHECK_SIZE_MAXINT32(stk.size());
}

INVESTABLEINDEXCORE_API int IIGetSecWeightCount(int indexobj, int date, int level)
{
	CHECK_INDEX_PTR;
	if (checkTradeDate(&date)) {
		return 0;
	}
	std::vector<long long> sctn;
	std::vector<double> weight;
	InterfaceC_IIIndex->getOpenSectionWeight(date, &sctn, &weight, level);
	return CHECK_SIZE_MAXINT32(sctn.size());
}

INVESTABLEINDEXCORE_API int IIGetSecWeight(int indexobj, int date, int* section, double* weights, int level)
{
	CHECK_INDEX_PTR;
	if (checkTradeDate(&date)) {
		return 0;
	}
	std::vector<long long> sctn;
	std::vector<double> weight;
	InterfaceC_IIIndex->getOpenSectionWeight(date, &sctn, &weight, level);
	auto stk_iter = sctn.begin();
	auto weight_iter = weight.begin();
	int* stk_ptr = section;
	double* weight_ptr = weights;
	for (; stk_iter != sctn.end(); ++stk_iter, ++weight_iter, ++stk_ptr, ++weight_ptr) {
		*stk_ptr = CHECK_CODE_MAX(*stk_iter);
		*weight_ptr = *weight_iter;
	}
	return CHECK_SIZE_MAXINT32(sctn.size());
}

INVESTABLEINDEXCORE_API int IIGetDayCount(int indexobj)
{
	CHECK_INDEX_PTR;
	return CHECK_SIZE_MAXINT32(InterfaceC_IIIndex->getDayCount());
}

INVESTABLEINDEXCORE_API int IIGetDates(int indexobj, int* dates)
{
	CHECK_INDEX_PTR;
	for (long long i = InterfaceC_IIIndex->getDayCount() - 1LL; i >= 0; --i) {
		dates[i] = CHECK_DATE_MAX(InterfaceC_IIIndex->getDateByIndex(i));
	}
	return CHECK_SIZE_MAXINT32(InterfaceC_IIIndex->getDayCount());
}

INVESTABLEINDEXCORE_API int IIGetDailyReturn(int indexobj, double* r)
{
	CHECK_INDEX_PTR;
	if (r != nullptr) {
		std::copy(InterfaceC_IIIndex->getDailyReturn().cbegin()
				, InterfaceC_IIIndex->getDailyReturn().cend()
				, r);
	}
	return CHECK_SIZE_MAXINT32(InterfaceC_IIIndex->getDailyReturn().size());
}

INVESTABLEINDEXCORE_API int IIGetCumReturn(int indexobj, double* r)
{
	CHECK_INDEX_PTR;
	if (r != nullptr) {
		std::copy(InterfaceC_IIIndex->getCumReturn().cbegin()
				, InterfaceC_IIIndex->getCumReturn().cend()
				, r);
	}
	return CHECK_SIZE_MAXINT32(InterfaceC_IIIndex->getCumReturn().size());
}

INVESTABLEINDEXCORE_API int IIGetDateByIndex(int indexobj, int index)
{
	CHECK_INDEX_PTR;
	return CHECK_DATE_MAX(InterfaceC_IIIndex->getDateByIndex(index));
}

INVESTABLEINDEXCORE_API int IIWriteReturnDaily(int indexobj, const char* file)
{
	CHECK_INDEX_PTR;
	InterfaceC_IIIndex->writeFileReturnDaily(file);
	return 0;
}

INVESTABLEINDEXCORE_API int IIWriteReturnYearly(int indexobj, const char* file)
{
	CHECK_INDEX_PTR;
	InterfaceC_IIIndex->writeFileReturnYearly(file);
	return 0;
}

INVESTABLEINDEXCORE_API int IIWriteWeightDay(int indexobj, const char* file, int date)
{
	CHECK_INDEX_PTR;
	if (checkTradeDate(&date)) {
		return 0;
	}
	InterfaceC_IIIndex->writeFileWeightDay(file, date);
	return 0;
}

INVESTABLEINDEXCORE_API int ProcEntry(HWND hwnd, HINSTANCE hinst, LPTSTR cmdline, int cmdshow)
{
	return InvestableIndex::InvestableIndexCUI();
}
