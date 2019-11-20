#include "pch.h"
#include "CppUnitTest.h"
#include "crc32.h"

#include "../InvestableIndexCore/DataColumn.h"
#include "../InvestableIndexCore/DataTable.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace InvestableIndexBaseUnitTest
{
	TEST_CLASS(DataColumnUnitTest)
	{
	public:
		const TCHAR* filename = _T("w:\\dataset\\calendarTRADE_DATE.dat");
		const int filesize = 20912;
		const unsigned int filehead = 0x1328cf9U;
		const unsigned int filecrc32 = 0x6785FB0DU;
		const long long filehead64 = 0x1328cf9;
		const long long filetail64 = 0x13416d2;

		TEST_METHOD(DataColumnInit)
		{
			InvestableIndex::DataColumn dc;
			int rs = dc.init(filename, 8);
			Assert::AreEqual(filesize / 8, rs);
		}

		TEST_METHOD(DataColumnGetBuffer)
		{
			InvestableIndex::DataColumn dc;
			Assert::AreEqual(0, reinterpret_cast<int>(dc.getBuffer()));

			int rs = dc.init(filename, 8);

			Assert::AreEqual(filesize, static_cast<int>(dc.getBufferSize()));
			Assert::AreEqual(filehead, *reinterpret_cast<const unsigned int *>(dc.getBuffer()));
			Assert::AreEqual(filecrc32, crc32(dc.getBuffer(), filesize));
		}

		TEST_METHOD(DataColumnGetLL)
		{
			InvestableIndex::DataColumn dc;
			Assert::AreEqual(0, reinterpret_cast<int>(dc.getll()));

			int rs = dc.init(filename, 8);

			Assert::AreEqual(filehead64, *dc.getll());
			Assert::AreEqual(filehead64, dc.getll(0));
			Assert::AreEqual(filesize / 8LL, dc.count());
			Assert::AreEqual(filetail64, dc.getll(dc.count() - 1));
		}
	};

	TEST_CLASS(DataTableUnitTest)
	{
	public:
		TEST_METHOD(DataTableInit)
		{
			InvestableIndex::DataTable dt;
			Assert::AreEqual(0, 0);
		}
	};
}
