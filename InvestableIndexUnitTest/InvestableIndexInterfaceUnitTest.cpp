#include "pch.h"
#include "CppUnitTest.h"
#include "crc32.h"

#include "../InvestableIndexCore/framework.h"
#include "../InvestableIndexCore/IndexInterface.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace InvestableIndexInterfaceUnitTest
{
	TEST_CLASS(IndexInterface)
	{
	public:

		TEST_METHOD(IndexInterfaceSimulate)
		{
			InvestableIndex::IndexInterface *index = InvestableIndex::IndexInterface::get();
			Assert::IsFalse(nullptr == index, L"create index interface failed");
			std::map<int, double> indexpt;
			index->simulate();
			index->writeMapReturnDaily(&indexpt);
			Assert::AreEqual(2681, static_cast<int>(indexpt[20191101]));
			index->setFixedDate(1);
			index->setTopStkCount(30);
			index->simulate();
			index->writeMapReturnDaily(&indexpt);
			Assert::AreEqual(3670, static_cast<int>(indexpt[20191101]));
			index->setTopStkWeight(10);
			index->simulate();
			index->writeMapReturnDaily(&indexpt);
			Assert::AreEqual(3638, static_cast<int>(indexpt[20191101]));
		}

	};

}
