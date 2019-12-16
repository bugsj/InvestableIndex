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
		const TCHAR* filename = _T("\\\\172.17.70.100\\个人文件夹\\luojie\\dataset\\calendarTRADE_DATE.dat");
		const long long filesize = 20912;
		const unsigned int filehead = 0x1328cf9U;
		const unsigned int filecrc32 = 0x6785FB0DU;
		const long long filehead64 = 0x1328cf9;
		const long long filetail64 = 0x13416d2;

		TEST_METHOD(DataColumnInit)
		{
			InvestableIndex::DataColumn dc(filename, 8);
			Assert::AreEqual(filesize / 8, dc.count());
		}

		TEST_METHOD(DataColumnGetBuffer)
		{
			InvestableIndex::DataColumn dc(filename, 8);
			Assert::AreEqual(filesize, dc.getBufferSize());
			Assert::AreEqual(filehead, *reinterpret_cast<const unsigned int *>(dc.getBuffer()));
			Assert::AreEqual(filecrc32, crc32(dc.getBuffer(), filesize));
		}

		TEST_METHOD(DataColumnGetLL)
		{
			InvestableIndex::DataColumn dc(filename, 8);
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
