///***************************************************************************
// File:	FRAMEWRK.C
//
// Purpose:	Framework library for Microsoft Excel.
//
//     This library provides some basic functions
//     that help in writing Excel DLLs. It includes
//     simple functions for managing memory with XLOPER12s,
//     creating temporary XLOPER12s, robustly calling
//     Excel12(), and outputting debugging strings
//     to the debugger for the current application.
//   
//     The main purpose of this library is to help
//     you to write cleaner C code for calling Excel.
//     For example, using the framework library you
//     can write
//   
//         Excel12f(xlcDisplay, 0, 2, TempMissing12(), TempBool12(0));
//   
//     instead of the more verbose
//   
//         XLOPER12 xMissing, xBool;
//         xMissing.xltype = xltypeMissing;
//         xBool.xltype = xltypeBool;
//         xBool.val.xbool = 0;
//         Excel12(xlcDisplay, 0, 2, (LPXLOPER12) &xMissing, (LPXLOPER12) &xBool);
//   
//   
//     The library is non-reentrant.
//   
//     Define _DEBUG to use the debugging functions.
//   
//     Source code is provided so that you may
//     enhance this library or optimize it for your
//     own application.
// 
// Platform:    Microsoft Windows
//
// Functions:		
//              debugPrintf
//              GetTempMemory
//              FreeAllTempMemory
//              Excel
//              Excel12f
//              TempNum
//              TempNum12
//              TempStr
//              TempStrConst
//              TempStr12
//              TempBool
//              TempBool12
//              TempInt
//              TempInt12
//              TempErr
//              TempErr12
//              TempActiveRef
//              TempActiveRef12
//              TempActiveCell
//              TempActiveCell12
//              TempActiveRow
//              TempActiveRow12
//              TempActiveColumn
//              TempActiveColumn12
//              TempMissing
//              TempMissing12
//              InitFramework
//				QuitFramework
//
//***************************************************************************

#include "pch.h"

#include <windows.h>
#include <malloc.h>
#include <wchar.h>
#include "xlcall.h"
#include "framewrk.h"
#include "memorymanager.h"
#include <stdarg.h>


#ifdef _DEBUG

///***************************************************************************
// debugPrintf()
//
// Purpose: 
//      sends a string to the debugger for the current application. 
//
// Parameters:
//
//      LPSTR lpFormat  The format definition string
//      ...             The values to print
//
// Returns: 
//
// Comments:
//
///***************************************************************************

void cdecl debugPrintf(LPCSTR lpFormat, ...)
{
	char rgch[256];
	va_list argList;

	va_start(argList,lpFormat);
	wvsprintfA(rgch,lpFormat,argList);
	va_end(argList);
	OutputDebugStringA(rgch);
}

#endif /* DEBUG */

///***************************************************************************
// GetTempMemory()
//
// Purpose: 
//       Allocates temporary memory. Temporary memory
//       can only be freed in one chunk, by calling
//       FreeAllTempMemory(). This is done by Excel12f().
//
// Parameters:
//
//      size_t cBytes      How many bytes to allocate
//
// Returns: 
//
//      LPSTR           A pointer to the allocated memory,
//                      or 0 if more memory cannot be
//                      allocated. If this fails,
//                      check that MEMORYSIZE is big enough
//                      in MemoryPool.h and that you have
//                      remembered to call FreeAllTempMemory
//
// Comments:
//
//	Algorithm:
//       
//      The memory allocation algorithm is extremely
//      simple: on each call, allocate the next cBytes
//      bytes of a static memory buffer. If the buffer
//      becomes too full, simply fail. To free memory,
//      simply reset the pointer (vOffsetMemBlock)
//      back to zero. This memory scheme is very fast
//      and is optimized for the assumption that the
//      only thing you are using temporary memory
//      for is to hold arguments while you call Excel12f().
//      We rely on the fact that you will free all the
//      temporary memory at the same time. We also
//      assume you will not need more memory than
//      the amount required to hold a few arguments
//      to Excel12f().
// 
//      Note that the memory allocation algorithm
//      now supports multithreaded applications by
//      giving each unique thread its own static
//      block of memory. This is handled in the
//      MemoryManager.cpp file automatically.
//
///***************************************************************************

LPSTR GetTempMemory(size_t cBytes)
{
	return MGetTempMemory(cBytes);
}

///***************************************************************************
// FreeAllTempMemory()
//
// Purpose: 
//
//      Frees all temporary memory that has been allocated
//      for the current thread
//
// Parameters:
//
// Returns: 
//
// Comments:
//
//
///***************************************************************************

void FreeAllTempMemory(void)
{
	MFreeAllTempMemory();
}


///***************************************************************************
// Excel12f()
//
// Purpose: 
//      A fancy wrapper for the Excel12() function. It also
//      does the following:
//        
//      (1) Checks that none of the LPXLOPER12 arguments are 0,
//          which would indicate that creating a temporary XLOPER12
//          has failed. In this case, it doesn't call Excel12
//          but it does print a debug message.
//      (2) If an error occurs while calling Excel12,
//          print a useful debug message.
//      (3) When done, free all temporary memory.
//        
//      #1 and #2 require _DEBUG to be defined.
//
// Parameters:
//
//      int xlfn            Function number (xl...) to call
//      LPXLOPER12 pxResult Pointer to a place to stuff the result,
//                          or 0 if you don't care about the result.
//      int count           Number of arguments
//      ...                 (all LPXLOPER12s) - the arguments.
//
// Returns: 
//
//      A return code (Some of the xlret... values, as defined
//      in XLCALL.H, OR'ed together).
//
// Comments:
//
///***************************************************************************

int cdecl Excel12f(int xlfn, LPXLOPER12 pxResult, int count, ...)
{
	int xlret;
	va_list ppxArgs;

#ifdef _DEBUG
	LPXLOPER12 px;
	int i;

	va_start(ppxArgs, count);

	for (i=0; i<count; i++)
	{
		px = va_arg(ppxArgs, LPXLOPER12);

		if (px == NULL)
		{
			debugPrintf("Error! A zero parameter is being passed "
						"to Excel12f. Check memory...\r");
			FreeAllTempMemory();
			return xlretFailed;
		}
	}

	va_end(ppxArgs);

#endif /* DEBUG */

	va_start(ppxArgs, count);
	xlret = Excel12v(xlfn,pxResult,count,(LPXLOPER12 *)ppxArgs);
	va_end(ppxArgs);

#ifdef _DEBUG

	if (xlret != xlretSuccess)
	{
		debugPrintf("Error! Excel12(");

		if (xlfn & xlCommand)
			debugPrintf("xlCommand | ");
		if (xlfn & xlSpecial)
			debugPrintf("xlSpecial | ");
		if (xlfn & xlIntl)
			debugPrintf("xlIntl | ");
		if (xlfn & xlPrompt)
			debugPrintf("xlPrompt | ");

		debugPrintf("%u) callback failed:",xlfn & 0x0FFF);

		/* More than one error bit may be on */

		if (xlret & xlretAbort)
		{
			debugPrintf(" Macro Halted\r");
		}

		if (xlret & xlretInvXlfn)
		{
			debugPrintf(" Invalid Function Number\r");
		}

		if (xlret & xlretInvCount)
		{
			debugPrintf(" Invalid Number of Arguments\r");
		}

		if (xlret & xlretInvXloper)
		{
			debugPrintf(" Invalid XLOPER12\r");
		}

		if (xlret & xlretStackOvfl)
		{
			debugPrintf(" Stack Overflow\r");
		}

		if (xlret & xlretFailed)
		{
			debugPrintf(" Command failed\r");
		}

		if (xlret & xlretUncalced)
		{
			debugPrintf(" Uncalced cell\r");
		}

	}

#endif /* DEBUG */

	FreeAllTempMemory();

	return xlret;
}



///***************************************************************************
// TempNum12()
//
// Purpose: 
//      Creates a temporary numeric (IEEE floating point) XLOPER12.
//
// Parameters:
//
//      double d        The value
//
// Returns: 
//
//      LPXLOPER12      The temporary XLOPER12, or 0
//                      if GetTempMemory() failed.
//
// Comments:
//		
//
///***************************************************************************

LPXLOPER12 TempNum12(double d)
{
	LPXLOPER12 lpx;

	lpx = (LPXLOPER12) GetTempMemory(sizeof(XLOPER12));

	if (!lpx)
	{
		return 0;
	}

	lpx->xltype = xltypeNum;
	lpx->val.num = d;

	return lpx;
}

///***************************************************************************
// TempStr12()
//
// Purpose: 
//      Creates a temporary string XLOPER12 from a
//      unicode const string with a local copy in
//      temp memory
//
// Parameters:
//
//      XCHAR lpstr     The string, as a null-terminated
//                      unicode string. This function will
//                      count the bytes of the string
//                      and insert that count in the
//                      first byte of the temp string.
//
// Returns: 
//
//      LPXLOPER12      The temporary XLOPER12, or 0
//                      if GetTempMemory() failed.
//
// Comments:
//
//      (1) Fix for const string pointers being passed in to TempStr.
//          Note it assumes NO leading space
//
//      (2) Also note that XLOPER12 now uses unicode for the string
//          operators
//
//      (3) Will remove the null-termination on the string
//
//
//
// Note: TempStr12 is different from TempStr and is more like TempStrConst
// in its behavior. We have consciously made this choice and deprecated the
// behavior of TempStr going forward. Refer to the note in comment section
// for TempStr to better understand this design decision.
///***************************************************************************

LPXLOPER12 TempStr12(const XCHAR* lpstr)
{
	LPXLOPER12 lpx;
	XCHAR* lps;
	int len;

	len = lstrlenW(lpstr);

	lpx = (LPXLOPER12) GetTempMemory(sizeof(XLOPER12) + (len+1)*2);

	if (!lpx)
	{
		return 0;
	}

	lps = (XCHAR*)((CHAR*)lpx + sizeof(XLOPER12));

	lps[0] = (BYTE)len;
	//can't wcscpy_s because of removal of null-termination
	wmemcpy_s( lps+1, len+1, lpstr, len);
	lpx->xltype = xltypeStr;
	lpx->val.str = lps;

	return lpx;
}

///***************************************************************************
// TempBool12()
//
// Purpose: 
//      Creates a temporary logical (true/false) XLOPER12.
//
// Parameters:
//
//      BOOL b          0 - for a FALSE XLOPER12
//                      Anything else - for a TRUE XLOPER12
//
// Returns: 
//
//      LPXLOPER12      The temporary XLOPER12, or 0
//                      if GetTempMemory() failed.
//
// Comments:
//
///***************************************************************************

LPXLOPER12 TempBool12(BOOL b)
{
	LPXLOPER12 lpx;

	lpx = (LPXLOPER12) GetTempMemory(sizeof(XLOPER12));

	if (!lpx)
	{
		return 0;
	}

	lpx->xltype = xltypeBool;
	lpx->val.xbool = b?1:0;

	return lpx;
}


///***************************************************************************
// TempInt12()
//
// Purpose: 
//          Creates a temporary integer XLOPER12.
//
// Parameters:
//
//      int i           The integer
//
// Returns: 
//
//      LPXLOPER12      The temporary XLOPER12, or 0
//                      if GetTempMemory() failed.
//
// Comments:
//
//      Note that the int oper has increased in size from
//      short int up to int in the 12 opers
//
///***************************************************************************

LPXLOPER12 TempInt12(int i)
{
	LPXLOPER12 lpx;

	lpx = (LPXLOPER12) GetTempMemory(sizeof(XLOPER12));

	if (!lpx)
	{
		return 0;
	}

	lpx->xltype = xltypeInt;
	lpx->val.w = i;

	return lpx;
}


///***************************************************************************
// TempErr12()
//
// Purpose: 
//      Creates a temporary error XLOPER12.
//
// Parameters:
//
//      int err         The error code. One of the xlerr...
//                      constants, as defined in XLCALL.H.
//                      See the Excel user manual for
//                      descriptions about the interpretation
//                      of various error codes.
//
// Returns: 
//
//      LPXLOPER12      The temporary XLOPER12, or 0
//                      if GetTempMemory() failed.
//
// Comments:
//
//      Note the paramater has changed from a WORD to an int
//      in the new 12 operators
//
///***************************************************************************

LPXLOPER12 TempErr12(int err)
{
	LPXLOPER12 lpx;

	lpx = (LPXLOPER12) GetTempMemory(sizeof(XLOPER12));

	if (!lpx)
	{
		return 0;
	}

	lpx->xltype = xltypeErr;
	lpx->val.err = err;

	return lpx;
}


///***************************************************************************
// TempActiveRef12()
//
// Purpose: 
//      Creates a temporary rectangular reference to the active
//      sheet. Remember that the active sheet is the sheet that
//      the user sees in front, not the sheet that is currently
//      being calculated.
//
// Parameters:
//
//      RW rwFirst      (0 based) The first row in the rectangle.
//      RW rwLast       (0 based) The last row in the rectangle.
//      COL colFirst    (0 based) The first column in the rectangle.
//      COL colLast     (0 based) The last column in the rectangle.
//
// Returns: 
//
//      LPXLOPER12      The temporary XLOPER12, or 0
//                      if GetTempMemory() failed.
//
// Comments:
//
//      Note that the formal parameters have changed for Excel 2007
//      The valid size has increased to accomodate the increase 
//      in Excel 2007 workbook sizes
//
///***************************************************************************

LPXLOPER12 TempActiveRef12(RW rwFirst,RW rwLast,COL colFirst,COL colLast)
{
	LPXLOPER12 lpx;
	LPXLMREF12 lpmref;
	int wRet;

	lpx = (LPXLOPER12) GetTempMemory(sizeof(XLOPER12));
	lpmref = (LPXLMREF12) GetTempMemory(sizeof(XLMREF12));

	if (!lpmref)
	{
		return 0;
	}

	/* calling Excel12f() instead of Excel12() would free all temp memory! */
	wRet = Excel12(xlSheetId, lpx, 0);

	if (wRet != xlretSuccess)
	{
		return 0;
	}
	else
	{
		lpx->xltype = xltypeRef;
		lpx->val.mref.lpmref = lpmref;
		lpmref->count = 1;
		lpmref->reftbl[0].rwFirst = rwFirst;
		lpmref->reftbl[0].rwLast = rwLast;
		lpmref->reftbl[0].colFirst = colFirst;
		lpmref->reftbl[0].colLast = colLast;

		return lpx;
	}
}


///***************************************************************************
// TempActiveCell12()
//
// Purpose: 
//      Creates a temporary reference to a single cell on the active
//      sheet. Remember that the active sheet is the sheet that
//      the user sees in front, not the sheet that is currently
//      being calculated.
//
// Parameters:
//
//      RW rw           (0 based) The row of the cell.
//      COL col         (0 based) The column of the cell.
//
// Returns: 
//
//      LPXLOPER12      The temporary XLOPER12, or 0
//                      if GetTempMemory() failed.
//
// Comments:
//
//      Paramter types changed to RW and COL to accomodate the increase
//      in sheet sizes introduced in Excel 2007
//
///***************************************************************************

LPXLOPER12 TempActiveCell12(RW rw, COL col)
{
	return TempActiveRef12(rw, rw, col, col);
}


///***************************************************************************
// TempActiveRow12()
//
// Purpose: 
//      Creates a temporary reference to an entire row on the active
//      sheet. Remember that the active sheet is the sheet that
//      the user sees in front, not the sheet that is currently
//      being calculated.
//
// Parameters:
//
//      RW rw           (0 based) The row.
//
// Returns: 
//
//      LPXLOPER12      The temporary XLOPER12, or 0
//                      if GetTempMemory() failed.
//
// Comments:
//
//      Paramter type change to RW to accomodate the increase in sheet
//      sizes introduced in Excel 2007
//
///***************************************************************************

LPXLOPER12 TempActiveRow12(RW rw)
{
	return TempActiveRef12(rw, rw, 0, 0x00003FFF);
}


///***************************************************************************
// TempActiveColumn12()
//
// Purpose: 
//      Creates a temporary reference to an entire column on the active
//      sheet. Remember that the active sheet is the sheet that
//      the user sees in front, not the sheet that is currently
//      being calculated.
//
// Parameters:
//
//      COL col	        (0 based) The column.
//
// Returns: 
//
//      LPXLOPER12      The temporary XLOPER12, or 0
//                      if GetTempMemory() failed.
//
// Comments:
//
//      Paramter type change to COL to accomodate the increase in sheet
//      sizes introduced in Excel 2007
//
///***************************************************************************

LPXLOPER12 TempActiveColumn12(COL col)
{
	return TempActiveRef12(0, 0x000FFFFF, col, col);
}

///***************************************************************************
// TempMissing12()
//
// Purpose: 
//      This is used to simulate a missing argument when
//      calling Excel12f(). It creates a temporary
//      "missing" XLOPER12.
//
// Parameters:
//
// Returns: 
//
//      LPXLOPER12      The temporary XLOPER12, or 0
//                      if GetTempMemory() failed.
//
// Comments:
//
///***************************************************************************

LPXLOPER12 TempMissing12(void)
{
	LPXLOPER12 lpx;

	lpx = (LPXLOPER12) GetTempMemory(sizeof(XLOPER12));

	if (!lpx)
	{
		return 0;
	}

	lpx->xltype = xltypeMissing;

	return lpx;
}

///***************************************************************************
// InitFramework()
//
// Purpose: 
//      Initializes all the framework functions.
//
// Parameters:
//
// Returns: 
//
// Comments:
//
///***************************************************************************

void InitFramework(void)
{
	FreeAllTempMemory();
}

///***************************************************************************
// QuitFramework()
//
// Purpose: 
//      Cleans up for all framework functions.
//
// Parameters:
//
// Returns: 
//
// Comments:
//
///***************************************************************************
void QuitFramework(void)
{
	FreeAllTempMemory();
}



///***************************************************************************
// FreeXLOper12T()
//
// Purpose: 
//      Will free any malloc'd memory associated with the given
//      LPXLOPER12, assuming it has any memory associated with it
//
// Parameters:
//
//      LPXLOPER12 pxloper12    Pointer to the XLOPER12 whose
//         	                    associated memory we want to free
//
// Returns: 
//
// Comments:
//
///***************************************************************************


void FreeXLOper12T(LPXLOPER12 pxloper12)
{
	DWORD xltype;
	int cxloper12;
	LPXLOPER12 pxloper12Free;

	xltype = pxloper12->xltype;

	switch (xltype)
	{
	case xltypeStr:
		if (pxloper12->val.str != NULL)
			free(pxloper12->val.str);
		break;
	case xltypeRef:
		if (pxloper12->val.mref.lpmref != NULL)
			free(pxloper12->val.mref.lpmref);
		break;
	case xltypeMulti:
		cxloper12 = pxloper12->val.array.rows * pxloper12->val.array.columns;
		if (pxloper12->val.array.lparray)
		{
			pxloper12Free = pxloper12->val.array.lparray;
			while (cxloper12 > 0)
			{
				FreeXLOper12T(pxloper12Free);
				pxloper12Free++;
				cxloper12--;
			}
			free(pxloper12->val.array.lparray);
		}
		break;
	case xltypeBigData:
		if (pxloper12->val.bigdata.h.lpbData != NULL)
			free(pxloper12->val.bigdata.h.lpbData);
		break;
	}
}
