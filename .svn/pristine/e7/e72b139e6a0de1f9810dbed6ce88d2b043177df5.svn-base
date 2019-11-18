#pragma once

#ifdef INVESTABLEINDEXCORE_EXPORTS
#define INVESTABLEINDEXCORE_API __declspec(dllexport)
#else
#define INVESTABLEINDEXCORE_API __declspec(dllimport)
#endif

#ifndef ERR_NOPOOLS
#define ERR_NOPOOLS (-1)
#endif

#ifdef __cplusplus
extern "C" {
#endif

	INVESTABLEINDEXCORE_API	int IIInit();
	INVESTABLEINDEXCORE_API	int IIRelease();

	INVESTABLEINDEXCORE_API int IISetIndexBase(int indexobj, int base);
	INVESTABLEINDEXCORE_API int IIGetIndexBase(int indexobj);
	INVESTABLEINDEXCORE_API int IISetStartDate(int indexobj, int date);
	INVESTABLEINDEXCORE_API int IISetEndDate(int indexobj, int date);
	INVESTABLEINDEXCORE_API int IISetFixDateFlag(int indexobj, int flag);
	INVESTABLEINDEXCORE_API int IISetTopStkWeight(int indexobj, int weight);
	INVESTABLEINDEXCORE_API int IISetTopStkCount(int indexobj, int cnt);
	INVESTABLEINDEXCORE_API int IISetIndexBasePool(int indexobj, int base);
	INVESTABLEINDEXCORE_API int IIAppendCSVPool(int indexobj, const char* file);
	INVESTABLEINDEXCORE_API int IIAppendSimplePool(int indexobj, int size, const int *buf);
	INVESTABLEINDEXCORE_API int IIAppendSectionPool(int indexobj, int section);
	INVESTABLEINDEXCORE_API int IIRemovePools(int indexobj);
	INVESTABLEINDEXCORE_API int IIGetWeightCount(int indexobj, int date);
	INVESTABLEINDEXCORE_API int IIGetWeight(int indexobj, int date, int *stks, double *weights);
	INVESTABLEINDEXCORE_API int IIGetSecWeightCount(int indexobj, int date, int level);
	INVESTABLEINDEXCORE_API int IIGetSecWeight(int indexobj, int date, int* section, double* weights, int level);
	INVESTABLEINDEXCORE_API int IIGetDayCount(int indexobj);
	INVESTABLEINDEXCORE_API int IIGetDates(int indexobj, int *dates);
	INVESTABLEINDEXCORE_API int IIGetDailyReturn(int indexobj, double *r);
	INVESTABLEINDEXCORE_API int IIGetCumReturn(int indexobj, double *r);
	INVESTABLEINDEXCORE_API int IIGetDateByIndex(int indexobj, int index);
	INVESTABLEINDEXCORE_API int IIWriteReturnDaily(int indexobj, const char* file);
	INVESTABLEINDEXCORE_API int IIWriteReturnYearly(int indexobj, const char* file);
	INVESTABLEINDEXCORE_API int IIWriteWeightDay(int indexobj, const char* file, int date);

	INVESTABLEINDEXCORE_API int IISimulate(int indexobj);

	INVESTABLEINDEXCORE_API int ProcEntry(HWND hwnd, HINSTANCE hinst, LPTSTR cmdline, int cmdshow);


#ifdef __cplusplus
}
#endif
