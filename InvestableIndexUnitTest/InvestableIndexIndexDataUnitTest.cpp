#include "pch.h"
#include "CppUnitTest.h"
#include "crc32.h"

#include "../InvestableIndexCore/framework.h"
#include "../InvestableIndexCore/DataSet.h"
#include "../InvestableIndexCore/IndexParameters.h"
#include "../InvestableIndexCore/IndexData.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace InvestableIndexInterfaceUnitTest
{
	TEST_CLASS(IndexData)
	{
	public:

		TEST_METHOD(IndexDataGetWeight)
		{
			std::unique_ptr<InvestableIndex::DataSet> data = InvestableIndex::DataSet::create();
			InvestableIndex::IndexParameters params;
			params.init(*data);
			InvestableIndex::IndexData index(*data, params);
			
			std::vector<long long> stk;
			std::vector<double> weight;
	
			size_t size;

			size = index.getWeight(20190930, &stk, &weight, InvestableIndex::DataTableColumn::PRE_PRICE);
			Assert::AreNotEqual(0ULL, size);
			Assert::AreEqual(size, stk.size());
			Assert::AreEqual(size, weight.size());
			Assert::AreEqual(1.0, std::accumulate(weight.begin(), weight.end(), 0.0), 0.01);
			Assert::IsTrue(*std::min_element(weight.begin(), weight.end()) > 0.0, L"all > 0");

			size = index.getWeight(20190930, &stk, &weight, InvestableIndex::DataTableColumn::CLOSE_PRICE);
			Assert::AreNotEqual(0ULL, size);
			Assert::AreEqual(size, stk.size());
			Assert::AreEqual(size, weight.size());
			Assert::AreEqual(1.0, std::accumulate(weight.begin(), weight.end(), 0.0), 0.01);
			Assert::IsTrue(*std::min_element(weight.begin(), weight.end()) > 0.0, L"all > 0");

			index.simulate();
			
			size = index.getWeight(20190930, &stk, &weight, InvestableIndex::DataTableColumn::PRE_PRICE);
			Assert::AreNotEqual(0ULL, size);
			Assert::AreEqual(size, stk.size());
			Assert::AreEqual(size, weight.size());
			Assert::AreEqual(1.0, std::accumulate(weight.begin(), weight.end(), 0.0), 0.01);
			Assert::IsTrue(*std::min_element(weight.begin(), weight.end()) > 0.0, L"all > 0");

			size = index.getWeight(20190930, &stk, &weight, InvestableIndex::DataTableColumn::CLOSE_PRICE);
			Assert::AreNotEqual(0ULL, size);
			Assert::AreEqual(size, stk.size());
			Assert::AreEqual(size, weight.size());
			Assert::AreEqual(1.0, std::accumulate(weight.begin(), weight.end(), 0.0), 0.01);
			Assert::IsTrue(*std::min_element(weight.begin(), weight.end()) > 0.0, L"all > 0");
		}

	};

}
