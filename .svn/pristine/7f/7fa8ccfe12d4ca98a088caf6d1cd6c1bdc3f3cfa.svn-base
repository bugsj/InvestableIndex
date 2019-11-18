#include "pch.h"
#include "CppUnitTest.h"

#include "../InvestableIndexCore/DataColumn.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace InvestableIndexUnitTest
{
	TEST_CLASS(DataColumnUnitTest)
	{
	public:
		
		TEST_METHOD(DataColumnInit)
		{
			DataColumn dc;
			int rs = dc.init(_T("w:\\dataset\\calendarTRADE_DATE.dat"), 8);
			Assert::AreEqual(20912 / 8, rs);
		}
	};
}
