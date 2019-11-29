#include "pch.h"
#include "CppUnitTest.h"
#include "crc32.h"

#include "../InvestableIndexCore/framework.h"
#include "../InvestableIndexCore/StringFormatter.h"
#include "../InvestableIndexCore/StkPool.h"
#include "../InvestableIndexCore/SimplePool.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace InvestableIndexPoolsUnitTest
{
	TEST_CLASS(SimplePoolUnitTest)
	{
	public:

		TEST_METHOD(SimplePoolSetGet)
		{
			InvestableIndex::Tools::StringFormatter<wchar_t> f;

			InvestableIndex::SimplePool p;
			InvestableIndex::StkPool& pool = p;
			std::vector<int> stki({ 1, 2, 3, 4, 5 });
			std::vector<long long> stk(stki.size());
			std::vector<long long> obj;
			std::copy(stki.begin(), stki.end(), stk.begin());

			p.set(stk.size(), stk.data());
			pool.getSnapshot(20190930, &obj);
			Assert::AreEqual(f.join(stk), f.join(obj));

			obj.clear();
			p.set(stki.size(), stki.data());
			pool.getSnapshot(20190930, &obj);
			Assert::AreEqual(f.join(stk), f.join(obj));

			stki.pop_back();
			stk.resize(stki.size());
			std::copy(stki.begin(), stki.end(), stk.begin());
			p.set(stki.size(), stki.data());
			pool.getSnapshot(20190930, &obj);
			Assert::AreEqual(f.join(stk), f.join(obj));

		}

	};

}
