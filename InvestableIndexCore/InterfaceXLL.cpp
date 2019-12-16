#include "pch.h"

#include "xlcall.h"
#include "FRAMEWRK.H"

#include "tools.h"
#include "IndexInterface.h"

extern "C" {
	__declspec(dllexport) int xlAutoOpen(void);
	__declspec(dllexport) int xlAutoClose(void);
	__declspec(dllexport) int xlAutoAdd(void);
	__declspec(dllexport) int xlAutoRemove(void);
	__declspec(dllexport) void xlAutoFree12(LPXLOPER12 p_oper);
	__declspec(dllexport) LPXLOPER12 xlAddInManagerInfo12(LPXLOPER12 xAction);
	__declspec(dllexport) LPXLOPER12 xlAutoRegister12(LPXLOPER12 pxName);
	__declspec(dllexport) LPXLOPER12 XLLdateDigit2Excel(LPXLOPER12 n);
	__declspec(dllexport) LPXLOPER12 XLLdateExcel2Digit(LPXLOPER12 n);
	__declspec(dllexport) void XLLHandleCancelEvent(void);

	__declspec(dllexport) void XLLInvestableIndex(
		  LPXLOPER12 begindate, LPXLOPER12 enddate
		, LPXLOPER12 basept, LPXLOPER12 basepool, LPXLOPER12 pool
		, LPXLOPER12 topweight, LPXLOPER12 topstks
		, LPXLOPER12 async_handle
	);
	__declspec(dllexport) LPXLOPER12 XLLInvestableIndexPoint(LPXLOPER12 idx, LPXLOPER12 date);
	__declspec(dllexport) LPXLOPER12 XLLInvestableIndexWeight(LPXLOPER12 idx, LPXLOPER12 xldate);
	__declspec(dllexport) LPXLOPER12 XLLInvestableIndexWType(LPXLOPER12 idx, LPXLOPER12 xltype);

}

using InvestableIndex::IndexInterface;
using InvestableIndex::WeightType;

int excelstr2int(XCHAR* str);

HMODULE XLL_hInst = 0;
HANDLE logfile = 0;



void XLLEntry(HMODULE hModule, DWORD ul_reason_for_call)
{
	switch (ul_reason_for_call)
	{
	case DLL_PROCESS_ATTACH:
		XLL_hInst = hModule;
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

inline void writelog(const char* log)
{
	InvestableIndex::Debug::WriteLog(log);
}

constexpr int g_rgWorksheetFuncsRows = 6;
constexpr int g_rgWorksheetFuncsArgCol = 10;
constexpr int g_rgWorksheetFuncsCols = g_rgWorksheetFuncsArgCol + 10;

#define ARGS_LIST_END (L"")
#define FUNC_CATEGORY_NAME (L"指数开发平台")

LPCWSTR g_rgWorksheetFuncs[g_rgWorksheetFuncsRows][g_rgWorksheetFuncsCols] =
{
	{ L"XLLdateDigit2Excel",		//Procedure
		L"QQ",			//type_text
		L"dateDigit2Excel",		//function_text
		L"日期",//argument_text
		L"1",//macro_type
		FUNC_CATEGORY_NAME,//category
		L"",//shortcut_text
		L"",//help_topic
		L"日期转为Excel日期",//function_help
		L"数字日期",//argument_help1
		ARGS_LIST_END
	},
	{ L"XLLdateExcel2Digit",		//Procedure
		L"QQ",			//type_text
		L"dateExcel2Digit",		//function_text
		L"日期",//argument_text
		L"1",//macro_type
		FUNC_CATEGORY_NAME,//category
		L"",//shortcut_text
		L"",//help_topic
		L"日期转为数字日期",//function_help
		L"日期",//argument_help1
		ARGS_LIST_END
	},
	{ L"XLLInvestableIndex",		//Procedure
		L">QQQQQQQX",			//type_text
		L"InvestableIndex",		//function_text
		L"起始日期,截止日期,基点,基础池(指数),股票池,最大个股权重,最大股票数",//argument_text
		L"1",//macro_type
		FUNC_CATEGORY_NAME,//category
		L"",//shortcut_text
		L"",//help_topic
		L"计算自定义指数",//function_help
		L"计算指数的起始日期",//argument_help1
		L"计算指数的截止日期",//argument_help2
		L"基期点位",//argument_help3
		L"基础股票池(指数)",//argument_help4
		L"指数选择的股票池",//argument_help5
		L"个股的最大权重",//argument_help6
		L"成分股最大股票数",//argument_help7
		ARGS_LIST_END
	},
	{ L"XLLInvestableIndexPoint",		//Procedure
		L"QQQ",			//type_text
		L"InvestableIndexPoint",		//function_text
		L"计算指数结果,日期",//argument_text
		L"1",//macro_type
		FUNC_CATEGORY_NAME,//category
		L"",//shortcut_text
		L"",//help_topic
		L"指定交易日指数点位",//function_help
		L"InvestableIndex返回的结果",//argument_help1
		L"交易日",//argument_help2
		ARGS_LIST_END
	},
	{ L"XLLInvestableIndexWeight",		//Procedure
		L"QQQ",			//type_text
		L"InvestableIndexWeight",		//function_text
		L"计算指数结果,日期",//argument_text
		L"1",//macro_type
		FUNC_CATEGORY_NAME,//category
		L"",//shortcut_text
		L"",//help_topic
		L"指定交易日成分股及权重",//function_help
		L"InvestableIndex返回的结果",//argument_help1
		L"交易日",//argument_help2
		ARGS_LIST_END
	},
	{ L"XLLInvestableIndexWType",		//Procedure
		L"QQQ",			//type_text
		L"InvestableIndexWType",		//function_text
		L"计算指数,权重类型",//argument_text
		L"1",//macro_type
		FUNC_CATEGORY_NAME,//category
		L"",//shortcut_text
		L"",//help_topic
		L"设置后台指数计算权重类型",//function_help
		L"InvestableIndex指数对象",//argument_help1
		L"权重类型",//argument_help2
		ARGS_LIST_END
	},
};


int Excel12Register(LPXLOPER12 dll, LPCWSTR *args)
{
	writelog("Excel12Register");
	int count;
	for (count = g_rgWorksheetFuncsArgCol; count < g_rgWorksheetFuncsCols; ++count) {
		if (args[count][0] == L'\0') {
			break;
		}
	}
	LPXLOPER12* ppxArgs = new LPXLOPER12[count + 1LL];

	ppxArgs[0] = dll;
	LPXLOPER12* oper_ptr = ppxArgs + 1;
	LPCWSTR* str_ptr = args;
	for (int i = count; i > 0; --i, ++oper_ptr, ++str_ptr) {
		*oper_ptr = TempStr12(*str_ptr);
	}

	int xlret = Excel12v(xlfRegister, 0, count + 1, ppxArgs);

	FreeAllTempMemory();
	delete[] ppxArgs;

	return xlret;
}

__declspec(dllexport) int xlAutoOpen(void)
{
	//
	// In the following block of code the name of the XLL is obtained by
	// calling xlGetName. This name is used as the first argument to the
	// REGISTER function to specify the name of the XLL. Next, the XLL loops
	// through the g_rgWorksheetFuncs[] table, and the g_rgCommandFuncs[]
	// tableregistering each function in the table using xlfRegister. 
	// Functions must be registered before you can add a menu item.
	//
	writelog("xlAutoOpen");
	XLOPER12 xDLL;	   // name of this DLL //
	Excel12f(xlGetName, &xDLL, 0);

	for (int i = 0; i < g_rgWorksheetFuncsRows; i++) {
		Excel12Register(&xDLL, g_rgWorksheetFuncs[i]);
	}
	Excel12f(xlEventRegister, 0, 2, TempStr12(L"XLLHandleCancelEvent"), TempInt12(xleventCalculationCanceled));

	Excel12f(xlFree, 0, 1, &xDLL);
	return 1;
}

__declspec(dllexport) int xlAutoClose(void)
{
	writelog("xlAutoClose");
	for (int i = 0; i < g_rgWorksheetFuncsRows; i++)
		Excel12f(xlfSetName, 0, 1, TempStr12(g_rgWorksheetFuncs[i][2]));

	return 1;
}

__declspec(dllexport) int xlAutoAdd(void)
{
	writelog("xlAutoAdd");
	return 1;
}

__declspec(dllexport) int xlAutoRemove(void)
{
	writelog("xlAutoRemove");
	return 1;
}

static const concurrency::task<void> empty_task;
static concurrency::task<void> calc_task;
static concurrency::cancellation_token_source cts;
static std::mutex calc_job_mutex;

void XLLHandleCancelEvent()
{
	writelog("XLLHandleCancelEvent");
	if (calc_task != empty_task && !calc_task.is_done()) {
		cts.cancel();
		calc_task.wait();
	}
}


LPXLOPER12 CreateXLOPERDLLFree(DWORD type)
{
	LPXLOPER12 obj = new XLOPER12;
	memset(obj, 0, sizeof(*obj));
	obj->xltype = (type | xlbitDLLFree);

	return obj;
}

void FreeXLOPERDLLFree(LPXLOPER12 obj, bool delete_root = true)
{
	if (obj->xltype == (xltypeMulti | xlbitDLLFree)) {
		int cnt = obj->val.array.columns * obj->val.array.rows;
		LPXLOPER12 ptr = obj->val.array.lparray;
		LPXLOPER12 ptr_end = obj->val.array.lparray + cnt;
		for (; ptr < ptr_end; ++ptr)
		{
			FreeXLOPERDLLFree(ptr, false);
		}
		delete[] obj->val.array.lparray;
	}
	else if (obj->xltype == (xltypeStr | xlbitDLLFree)) {
		delete[] obj->val.str;
	}

	if (delete_root && (obj->xltype & xlbitDLLFree) != 0) {
		delete obj;
	}
}

LPXLOPER12 XLOPERNumDLLFree(double num)
{
	LPXLOPER12 obj = CreateXLOPERDLLFree(xltypeNum);
	obj->val.num = num;

	return obj;
}


LPXLOPER12 XLOPERErrDLLFree(int err)
{
	LPXLOPER12 obj = CreateXLOPERDLLFree(xltypeErr);
	obj->val.err = err;

	return obj;
}

LPXLOPER12 XLOPERMultiDLLFree(int col, int rw)
{
	LPXLOPER12 obj = CreateXLOPERDLLFree(xltypeMulti);
	obj->val.array.columns = col;
	obj->val.array.rows = rw;
	obj->val.array.lparray = new XLOPER12[static_cast<size_t>(col)* rw];
	memset(obj->val.array.lparray, 0, sizeof(XLOPER12) * static_cast<size_t>(col)* rw);

	return obj;
}

LPXLOPER12 XLOPERStrDLLFree(const XCHAR *str)
{
	LPXLOPER12 obj = CreateXLOPERDLLFree(xltypeStr);
	size_t len = wcslen(str);
	obj->val.str = new XCHAR[len + 4];
	obj->val.str[0] = static_cast<unsigned short>(len);
	
	wcsncpy_s(obj->val.str + 1, len + 1, str, len);

	return obj;
}

LPXLOPER12 setXLOPERStrDLLFree(LPXLOPER12 obj, const XCHAR* str)
{
	DEBUGBREAKCOND(obj->xltype == xltypeStr);
	DEBUGBREAKCOND(obj->xltype == xltypeMulti);
	FreeXLOPERDLLFree(obj, false);

	obj->xltype = (xltypeStr | xlbitDLLFree);
	size_t len = wcslen(str);
	obj->val.str = new XCHAR[len + 4];
	obj->val.str[0] = static_cast<unsigned short>(len);

	wcsncpy_s(obj->val.str + 1, len + 1, str, len);

	return obj;
}


__declspec(dllexport) void xlAutoFree12(LPXLOPER12 p_oper)
{
	writelog("xlAutoFree12");
	FreeXLOPERDLLFree(p_oper);
}

__declspec(dllexport) LPXLOPER12 xlAddInManagerInfo12(LPXLOPER12 xAction)
{
	writelog("xlAddInManagerInfo12");
	XLOPER12 xIntAction;

	DEBUGBREAKCOND(xAction->xltype != xltypeNum || xAction->val.num != 1.0);
	//
	// This code coerces the -in value to an integer. This is how the
	// code determines what is being requested. If it receives a 1, 
	// it returns a string representing the long name. If it receives 
	// anything else, it returns a #VALUE! error.
	//passed

	int action = 0;
	if (xAction->xltype == xltypeNum && fabs(xAction->val.num - 1.0) < 0.1) {
		action = 1;
	}
	else if (xAction->xltype == xltypeInt) {
		action = xAction->val.w;
	}
	else if (xAction->xltype == xltypeStr) {
		action = excelstr2int(xAction->val.str);
	}
	else {
		Excel12f(xlCoerce, &xIntAction, 2, xAction, TempInt12(xltypeInt));
		action = xIntAction.val.w;
	}

	if (action == 1)
	{
		writelog("xlAddInManagerInfo12 return name");
		return XLOPERStrDLLFree(FUNC_CATEGORY_NAME);
	}
	else
	{
		writelog("xlAddInManagerInfo12 return err");
		return XLOPERErrDLLFree(xlerrValue);
	}
}

int lpwstricmp(LPCWSTR s, LPCWSTR t)
{
	int i;

	if (wcslen(s) != *t)
		return 1;

	for (i = 1; i <= s[0]; i++)
	{
		if (towlower(s[i - 1]) != towlower(t[i]))
			return 1;
	}
	return 0;
}


__declspec(dllexport) LPXLOPER12 xlAutoRegister12(LPXLOPER12 pxName)
{
	writelog("xlAutoRegister12");
	XLOPER12 xDLL;
	LPXLOPER12 RegId;
	int i;

	//
	// This block initializes xRegId to a #VALUE! error first. This is done in
	// case a function is not found to register. Next, the code loops through 
	// the functions in g_rgFuncs[] and uses lpwstricmp to determine if the 
	// current row in g_rgFuncs[] represents the function that needs to be 
	// registered. When it finds the proper row, the function is registered 
	// and the register ID is returned to Microsoft Excel. If no matching 
	// function is found, an xRegId is returned containing a #VALUE! error.
	//

	RegId = XLOPERErrDLLFree(xlerrValue);

	for (i = 0; i < g_rgWorksheetFuncsRows; i++)
	{
		if (!lpwstricmp(g_rgWorksheetFuncs[i][0], pxName->val.str))
		{
			Excel12f(xlGetName, &xDLL, 0);
			Excel12Register(&xDLL, g_rgWorksheetFuncs[i]);
			Excel12f(xlFree, 0, 1, (LPXLOPER12)&xDLL);

			return RegId;
		}
	}

	return RegId;
}



int exceldate2date(int date)
{
	if (date > 10000000) {
		return date;
	}
	else {
		return InvestableIndex::fromExcelDate(date);
	}
}

int excelstr2int(XCHAR* str)
{
	int result = 0;
	XCHAR* ptr = str + 1;
	for (int i = 0; i <= *str; ++i, ++ptr) {
		if (iswdigit(*ptr)) {
			result = result * 10 + (*ptr - L'0');
		}
		else {
			break;
		}
	}
	return result;
}

int excelstr2date(XCHAR* str)
{
	int result = 0;
	XCHAR* ptr = str + 1;
	int w = 0;
	int part[3];
	for (int i = 0; i <= *str; ++i, ++ptr) {
		if (iswdigit(*ptr)) {
			result = result * 10 + (*ptr - L'0');
		}
		else {
			if (w > 2) {
				break;
			}
			if (result > 0) {
				part[w] = result;
				result = 0;
				++w;
			}
		}
	}
	if (w == 3) {
		return part[0] * 10000 + part[1] * 100 + part[2];
	}
	else {
		return result;
	}
}

int xloper2digitdate(LPXLOPER12 data, int *result)
{
	XLOPER12 xlt;
	xlt.xltype = 0;
	int error = -1;

	if (data->xltype == xltypeSRef || data->xltype == xltypeRef) {
		error = Excel12f(xlCoerce, &xlt, 1, data);
		if (!error) {
			error = -1;
			data = &xlt;
		}
	}
	if (data->xltype == xltypeMulti) {
		data = data->val.array.lparray;
	}
	switch (data->xltype) {
	case xltypeNum:
		*result = exceldate2date(static_cast<int>(data->val.num));
		break;
	case xltypeInt:
		*result = exceldate2date(data->val.w);
		break;
	case xltypeStr:
		*result = excelstr2date(data->val.str);
		break;
	default:
		error = xlerrValue;
		break;
	}

	if (xlt.xltype != 0) {
		Excel12f(xlFree, 0, 1, &xlt);
	}

	return error;
}


int xloper2int(LPXLOPER12 data, int* result)
{
	XLOPER12 xlt;
	xlt.xltype = 0;
	int error = -1;
	*result = 0;

	if (data->xltype == xltypeNil || data->xltype == xltypeMissing) {
		error = xlerrNA;
		*result = 0;
		return error;
	}
	if (data->xltype == xltypeSRef || data->xltype == xltypeRef) {
		error = Excel12f(xlCoerce, &xlt, 1, data);
		if (!error) {
			error = -1;
			data = &xlt;
		}
	}
	if (data->xltype == xltypeMulti) {
		data = data->val.array.lparray;
	}
	switch (data->xltype) {
	case xltypeNum:
		*result = static_cast<int>(data->val.num);
		break;
	case xltypeInt:
		*result = data->val.w;
		break;
	case xltypeStr:
		*result = excelstr2int(data->val.str);
		break;
	default:
		error = xlerrValue;
		break;
	}

	if (xlt.xltype != 0) {
		Excel12f(xlFree, 0, 1, &xlt);
	}

	return error;
}

int xloper2double(LPXLOPER12 data, double* result)
{
	XLOPER12 xlt;
	xlt.xltype = 0;
	int error = -1;

	if (data->xltype == xltypeNil || data->xltype == xltypeMissing) {
		error = xlerrNA;
		*result = 0;
		return error;
	}
	if (data->xltype == xltypeSRef || data->xltype == xltypeRef) {
		error = Excel12f(xlCoerce, &xlt, 1, data);
		if (!error) {
			error = -1;
			data = &xlt;
		}
	}
	if (data->xltype == xltypeMulti) {
		data = data->val.array.lparray;
	}
	switch (data->xltype) {
	case xltypeNum:
		*result = data->val.num;
		break;
	case xltypeInt:
		*result = data->val.w;
		break;
	default:
		error = xlerrValue;
		break;
	}

	if (xlt.xltype != 0) {
		Excel12f(xlFree, 0, 1, &xlt);
	}

	return error;
}

int xloper2stkpool(LPXLOPER12 data, std::vector<int>* result)
{
	XLOPER12 xlt;
	xlt.xltype = 0;
	int error = -1;

	int stk;
	result->clear();


	if (data->xltype == xltypeNil || data->xltype == xltypeMissing) {
		error = xlerrNA;
		return error;
	}
	if (data->xltype == xltypeSRef || data->xltype == xltypeRef) {
		error = Excel12f(xlCoerce, &xlt, 1, data);
		if (!error) {
			error = -1;
			data = &xlt;
		}
	}
	if (data->xltype == xltypeMulti) {
		int cnt = data->val.array.columns * data->val.array.rows;
		XLOPER12* ptr = data->val.array.lparray;
		for (; cnt > 0; --cnt, ++ptr) {
			error = xloper2int(ptr, &stk);
			if (error == -1 && stk != 0) {
				result->push_back(stk);
			}
		}
		if (!result->empty()) {
			error = -1;
		}
	}
	else {
		error = xloper2int(data, &stk);
		if (error == -1 && stk != 0) {
			result->push_back(stk);
		}
	}
	if (xlt.xltype != 0) {
		Excel12f(xlFree, 0, 1, &xlt);
	}

	return error;
}

__declspec(dllexport) LPXLOPER12 XLLdateDigit2Excel(LPXLOPER12 n)
{
	writelog("XLLdateDigit2Excel");
	int digitdate, error = -1;

	error = xloper2digitdate(n, &digitdate);

	if (error == -1) {
		return XLOPERNumDLLFree(digitdate > 1000000 ? InvestableIndex::toExcelDate(digitdate) : digitdate);
	}
	else
	{
		return XLOPERErrDLLFree(error);
	}
}

__declspec(dllexport) LPXLOPER12 XLLdateExcel2Digit(LPXLOPER12 n)
{
	writelog("XLLdateExcel2Digit");
	int serialdate, error = -1;

	error = xloper2digitdate(n, &serialdate);

	if (error == -1) {
		return XLOPERNumDLLFree(serialdate < 1000000 ? InvestableIndex::fromExcelDate(serialdate) : serialdate);
	}
	else
	{
		return XLOPERErrDLLFree(error);
	}
}


static std::map<int, double> IndexSimResult;
static int xll_ii_callcnt = 0;

__declspec(dllexport) void XLLInvestableIndex(
	LPXLOPER12 begindate, LPXLOPER12 enddate
	, LPXLOPER12 basept, LPXLOPER12 basepool, LPXLOPER12 pool
	, LPXLOPER12 topweight, LPXLOPER12 topstks
	, LPXLOPER12 async_handle
)
{
	writelog("XLLInvestableIndex");
	int error = 0;
	int bdate = 0, edate = 0, pt = 0, bpool = 0, topcnt = 0;
	double topw = 0;
	std::vector<int> stkpool;

	if (xloper2int(begindate, &bdate) != -1) {
		bdate = 0;
	}
	if (xloper2int(enddate, &edate) != -1) {
		edate = 0;
	}
	if (xloper2int(basept, &pt) != -1) {
		pt = -1;
	}
	if (xloper2int(basepool, &bpool) != -1) {
		bpool = -1;
	}
	if (xloper2stkpool(pool, &stkpool) != -1) {
		stkpool.clear();
	}
	if (xloper2double(topweight, &topw) != -1) {
		topw = -1;
	}
	if (xloper2int(topstks, &topcnt) != -1) {
		topcnt = -1;
	}

	concurrency::cancellation_token_source new_cts;
	cts = new_cts;
	concurrency::cancellation_token calc_task_token = cts.get_token();
	calc_task = concurrency::create_task([=] {
		writelog("XLLInvestableIndex Task");
		concurrency::cancellation_token cancel_token = calc_task_token;
		IndexInterface* index;
		try {
			index = IndexInterface::get();
		}
		catch (std::runtime_error e) {
			writelog("XLLInvestableIndex Task - CoreError");
			Excel12f(xlAsyncReturn, 0, 2, async_handle, TempErr12(xlerrNA));
			return;
		}
		if (bdate != 0) {
			index->setPeriodStartDate(bdate);
		}

		if (edate != 0) {
			index->setPeriodEndDate(edate);
		}

		if (pt >= 0) {
			index->setIndexBase(pt);
		}

		if (bpool >= 0) {
			if (bpool < 10) {
				index->setIndexBasePool(bpool);
			}
			else if (bpool == 300 || bpool == 399300) {
				index->setIndexBasePool(3);
			}
			else if (bpool == 905 || bpool == 399905) {
				index->setIndexBasePool(5);
			}
			else if (bpool == 985 || bpool == 399985) {
				index->setIndexBasePool(9);
			}
		}

		index->removePools();
		if (!stkpool.empty()) {
			index->appendSimplePool(stkpool.size(), stkpool.data());
		}

		if (topw >= 0) {
			index->setTopStkWeight(static_cast<long long>(topw * 100));
		}

		if (topcnt >= 0) {
			index->setTopStkCount(topcnt);
		}

		index->setFixedDate(1);
		{
			std::lock_guard<std::mutex> lck(calc_job_mutex);
			writelog("XLLInvestableIndex Task - Start");
			IndexInterface::get()->simulate(&cancel_token);
		}
		IndexInterface::get()->writeMapReturnDaily(&IndexSimResult);
		++xll_ii_callcnt;

		if (cancel_token.is_canceled()) {
			concurrency::cancel_current_task();
		};
		writelog("XLLInvestableIndex Task - Finish");
		Excel12f(xlAsyncReturn, 0, 2, async_handle, TempInt12(1));
		writelog("XLLInvestableIndex Task - Exit");
	}, calc_task_token);

	writelog("XLLInvestableIndex - Exit");
	return;
}

__declspec(dllexport) LPXLOPER12 XLLInvestableIndexPoint(LPXLOPER12 idx, LPXLOPER12 date)
{
	writelog("XLLInvestableIndexPoint");
	XLOPER12 xlt;
	xlt.xltype = 0;
	int error = -1;

	if (date->xltype == xltypeNil || date->xltype == xltypeMissing) {
		return XLOPERErrDLLFree(xlerrNA);
	}

	if (date->xltype == xltypeSRef || date->xltype == xltypeRef) {
		error = Excel12f(xlCoerce, &xlt, 1, date);
		if (!error) {
			error = -1;
			date = &xlt;
		}
	}

	LPXLOPER12 result;
	if (date->xltype == xltypeMulti) {
		result = XLOPERMultiDLLFree(date->val.array.columns, date->val.array.rows);

		int cnt = date->val.array.columns * date->val.array.rows;
		LPXLOPER12 source = date->val.array.lparray;
		LPXLOPER12 dest = result->val.array.lparray;
		for (; cnt > 0; --cnt, ++source, ++dest) {
			int ptdate = 0;
			int error = -1;
			error = xloper2digitdate(source, &ptdate);
			std::map<int, double>::iterator pt;
			pt = IndexSimResult.upper_bound(ptdate);
			if (error == -1 && pt != IndexSimResult.begin()) {
				dest->xltype = xltypeNum;
				dest->val.num = (--pt)->second;
			}
			else {
				dest->xltype = xltypeErr;
				dest->val.err = xlerrNA;
			}
		}
	}
	else {
		int ptdate = 0;
		int error = -1;
		error = xloper2digitdate(date, &ptdate);

		auto pt = IndexSimResult.upper_bound(ptdate);

		if (error == -1 && pt != IndexSimResult.begin()) {
			result = XLOPERNumDLLFree((--pt)->second);
		}
		else {
			result = XLOPERErrDLLFree(xlerrNA);
		}
	}

	if (xlt.xltype != 0) {
		Excel12f(xlFree, 0, 1, &xlt);
	}

	return result;
}

__declspec(dllexport) LPXLOPER12 XLLInvestableIndexWeight(LPXLOPER12 idx, LPXLOPER12 xldate)
{
	writelog("XLLInvestableIndexWeight - Start");

	IndexInterface* index;
	try {
		index = IndexInterface::get();
	}
	catch (std::runtime_error e) {
		writelog("XLLInvestableIndexWeight - CoreError");
		return XLOPERErrDLLFree(xlerrNA);
	}

	int date;
	int error = xloper2digitdate(xldate, &date);
	if (error != -1) {
		writelog("XLLInvestableIndexWeight - DateError");
		return XLOPERErrDLLFree(error);
	}

	int refdate = CHECK_DATE_MAX(index->getLastTradeDate(date));
	if (refdate > date) {
		writelog("XLLInvestableIndexWeight - Date out of range");
		return XLOPERErrDLLFree(xlerrNA);
	}
	else {
		date = refdate;
	}


	std::vector<long long> stks;
	std::vector<double> weight;
	std::vector<int> rank;

	long long cnt;
	if ((cnt = index->getOpenWeight(date, &stks, &weight)) == 0) {
		writelog("XLLInvestableIndexWeight - empty");
		return XLOPERErrDLLFree(xlerrNA);
	}
	DEBUGBREAKCOND(cnt != stks.size());
	DEBUGBREAKCOND(cnt != weight.size());

	rank.resize(cnt);
	std::iota(rank.begin(), rank.end(), 0);
	std::sort(rank.begin(), rank.end(), [&weight](int l, int r) { return weight[l] > weight[r]; });

	writelog("XLLInvestableIndexWeight - core result");

	LPXLOPER12 result = XLOPERMultiDLLFree(2, CHECK_SIZE_MAXINT32(cnt));
	
	LPXLOPER12 rs_iter = result->val.array.lparray;

	XCHAR stkcode[10] = L"000000.S?";
	for (auto iter = rank.begin(); iter != rank.end(); ++iter) {
		stkcode[5] = (stks[*iter]) % 10 + L'0'; stks[*iter] /= 10;
		stkcode[4] = (stks[*iter]) % 10 + L'0'; stks[*iter] /= 10;
		stkcode[3] = (stks[*iter]) % 10 + L'0'; stks[*iter] /= 10;
		stkcode[2] = (stks[*iter]) % 10 + L'0'; stks[*iter] /= 10;
		stkcode[1] = (stks[*iter]) % 10 + L'0'; stks[*iter] /= 10;
		stkcode[0] = (stks[*iter]) % 10 + L'0';
		stkcode[8] = (stks[*iter] < 5) ? L'Z' : L'H';
		setXLOPERStrDLLFree(rs_iter, stkcode);
		++rs_iter;
		rs_iter->xltype = xltypeNum;
		rs_iter->val.num = weight[*iter];
		++rs_iter;
	}
	writelog("XLLInvestableIndexWeight - Finish");
	return result;
}

__declspec(dllexport) LPXLOPER12 XLLInvestableIndexWType(LPXLOPER12 idx, LPXLOPER12 xltype)
{
	writelog("XLLInvestableIndexWType");
	
	IndexInterface* index;
	try {
		index = IndexInterface::get();
	}
	catch (std::runtime_error e) {
		writelog("XLLInvestableIndexWType - CoreError");
		return XLOPERErrDLLFree(xlerrNA);
	}

	int type;
	int error = xloper2int(xltype, &type);
	if (error != -1) {
		return XLOPERErrDLLFree(error);
	}

	writelog("XLLInvestableIndexWType - Set");
	if (IndexSimResult.empty() || !calc_job_mutex.try_lock()) {
		writelog("XLLInvestableIndexWType - Cancel");
		return XLOPERErrDLLFree(xlerrNA);
	}
	switch (type) {
	case 0:
		index->setWeightType(WeightType::FREESHARE);
		break;
	case 1:
		index->setWeightType(WeightType::TOTALSHARE);
		break;
	default:
		writelog("XLLInvestableIndexWType - ErrorValue");
		error = xlerrValue;
	}
	calc_job_mutex.unlock();

	writelog("XLLInvestableIndexWType - Exit");
	if (error != -1) {
		return XLOPERErrDLLFree(error);
	}
	return XLOPERNumDLLFree(0);
}
