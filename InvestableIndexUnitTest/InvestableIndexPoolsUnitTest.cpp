#include "pch.h"
#include "CppUnitTest.h"
#include "crc32.h"

#include "../InvestableIndexCore/framework.h"
#include "../InvestableIndexCore/StringFormatter.h"
#include "../InvestableIndexCore/StkPool.h"
#include "../InvestableIndexCore/SimplePool.h"
#include "../InvestableIndexCore/SimpleFilePool.h"
#include "../InvestableIndexCore/SetFilePool.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

template <typename T, std::size_t N>
constexpr std::size_t array_size(const T(&)[N]) { return N; }

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

	TEST_CLASS(SimpleFilePoolUnitTest)
	{
	public:

		TEST_METHOD(SimpleFilePoolSetGet)
		{
			InvestableIndex::Tools::StringFormatter<wchar_t> f;

			constexpr const char* file = "c:\\Users\\luojie\\Documents\\def.csv";
			constexpr int BUFSIZE = 256;

			InvestableIndex::SimpleFilePool p(file);
			InvestableIndex::StkPool& pool = p;
			std::vector<std::pair<long long, long long>> list;
			std::vector<char> buf(BUFSIZE);

			std::ifstream fin(file, std::ios::in);
			while (fin.getline(buf.data(), BUFSIZE)) {
				long long code, date;
				const char* ptr = buf.data();
				code = atoi(buf.data());
				while (*ptr != ',' && *ptr != '\n' && *ptr != '\0' && ptr < buf.data() + BUFSIZE) {
					++ptr;
				}
				if (*ptr == ',') {
					date = atoi(ptr + 1);
				}
				else {
					date = 0;
				}
				list.emplace_back(std::make_pair(code, date));
			}
			fin.close();

			constexpr const int testdate[] = { 20091231,20100321,20150630,20170730,20190930 };

			for (size_t i = 0; i < array_size(testdate); ++i) {
				std::vector<long long> expect, actual;
				expect.clear();
				for (auto stk : list) {
					if (stk.second <= testdate[i]) {
						expect.push_back(stk.first);
					}
				}
				p.getSnapshot(testdate[i], &actual);
				Assert::AreEqual(f.join(expect), f.join(actual));
			}

		}

	};

	TEST_CLASS(SetFilePoolUnitTest)
	{
	public:

		TEST_METHOD(SetFilePoolSetGet)
		{
			InvestableIndex::Tools::StringFormatter<wchar_t> f;

			constexpr const char* file = "c:\\Users\\luojie\\Documents\\def.csv";
			constexpr int BUFSIZE = 256;

			InvestableIndex::SimpleFilePool p(file);
			InvestableIndex::StkPool& pool = p;
			std::vector<std::pair<long long, long long>> list;
			std::vector<char> buf(BUFSIZE);

			std::ifstream fin(file, std::ios::in);
			while (fin.getline(buf.data(), BUFSIZE)) {
				long long code, date;
				const char* ptr = buf.data();
				code = atoi(buf.data());
				while (*ptr != ',' && *ptr != '\n' && *ptr != '\0' && ptr < buf.data() + BUFSIZE) {
					++ptr;
				}
				if (*ptr == ',') {
					date = atoi(ptr + 1);
				}
				else {
					date = 0;
				}
				list.emplace_back(std::make_pair(code, date));
			}
			fin.close();

			constexpr const int testdate[] = { 20091231,20100321,20150630,20170730,20190930 };

			for (size_t i = 0; i < array_size(testdate); ++i) {
				std::vector<long long> expect, actual;
				expect.clear();
				for (auto stk : list) {
					if (stk.second <= testdate[i]) {
						expect.push_back(stk.first);
					}
				}
				p.getSnapshot(testdate[i], &actual);
				Assert::AreEqual(f.join(expect), f.join(actual));
			}
		}
	};

}
