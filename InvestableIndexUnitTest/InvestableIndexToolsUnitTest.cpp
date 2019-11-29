#include "pch.h"
#include "CppUnitTest.h"
#include "crc32.h"

#include "../InvestableIndexCore/framework.h"
#include "../InvestableIndexCore/tools.h"
#include "../InvestableIndexCore/StringFormatter.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace InvestableIndexToolsUnitTest
{
	TEST_CLASS(BasicToolsUnitTest)
	{
	public:
		TEST_METHOD(NULLTermLengthUnitTest)
		{
			char* test_str[] = {
				"", "1", "abc", "һ������", "using namespace Microsoft::VisualStudio::CppUnitTestFramework;",
				"���ģʽ�ǽ�ģʽ��һ�����Ӧ��������������򣬼��������������ָ��˼·��������塣"
				"���ģʽ���ǽ��������ģʽ�������� �ܹ�ģʽ������ģʽ�͹���ģʽ�ȣ�"
				"ʵ���ϣ�����������ڵ�ÿһ���׶ζ�������һЩ����ͬ��ģʽ��"
			};
			wchar_t* test_wstr[] = {
				L"", L"1", L"abc", L"һ������", L"using namespace Microsoft::VisualStudio::CppUnitTestFramework;",
				L"���ģʽ�ǽ�ģʽ��һ�����Ӧ��������������򣬼��������������ָ��˼·��������塣"
				L"���ģʽ���ǽ��������ģʽ�������� �ܹ�ģʽ������ģʽ�͹���ģʽ�ȣ�"
				L"ʵ���ϣ�����������ڵ�ÿһ���׶ζ�������һЩ����ͬ��ģʽ��"
			};
			Assert::AreEqual(static_cast<long long>(strlen(test_str[0])), InvestableIndex::Tools::nulltermlen(test_str[0]));
			Assert::AreEqual(static_cast<long long>(strlen(test_str[1])), InvestableIndex::Tools::nulltermlen(test_str[1]));
			Assert::AreEqual(static_cast<long long>(strlen(test_str[2])), InvestableIndex::Tools::nulltermlen(test_str[2]));
			Assert::AreEqual(static_cast<long long>(strlen(test_str[3])), InvestableIndex::Tools::nulltermlen(test_str[3]));
			Assert::AreEqual(static_cast<long long>(strlen(test_str[4])), InvestableIndex::Tools::nulltermlen(test_str[4]));
			Assert::AreEqual(static_cast<long long>(strlen(test_str[5])), InvestableIndex::Tools::nulltermlen(test_str[5]));

			Assert::AreEqual(static_cast<long long>(wcslen(test_wstr[0])), InvestableIndex::Tools::nulltermlen(test_wstr[0]));
			Assert::AreEqual(static_cast<long long>(wcslen(test_wstr[1])), InvestableIndex::Tools::nulltermlen(test_wstr[1]));
			Assert::AreEqual(static_cast<long long>(wcslen(test_wstr[2])), InvestableIndex::Tools::nulltermlen(test_wstr[2]));
			Assert::AreEqual(static_cast<long long>(wcslen(test_wstr[3])), InvestableIndex::Tools::nulltermlen(test_wstr[3]));
			Assert::AreEqual(static_cast<long long>(wcslen(test_wstr[4])), InvestableIndex::Tools::nulltermlen(test_wstr[4]));
			Assert::AreEqual(static_cast<long long>(wcslen(test_wstr[5])), InvestableIndex::Tools::nulltermlen(test_wstr[5]));
		}

		TEST_METHOD(AppendStrUnitTest)
		{
			std::vector<char> buf;

			InvestableIndex::Tools::appendStr(&buf, "");
			Assert::AreEqual("", buf.data());

			InvestableIndex::Tools::appendStr(&buf, "");
			Assert::AreEqual("", buf.data());

			InvestableIndex::Tools::appendStr(&buf, "123");
			Assert::AreEqual("123", buf.data());

			InvestableIndex::Tools::appendStr(&buf, "123");
			Assert::AreEqual("123123", buf.data());

			buf.resize(0);
			InvestableIndex::Tools::appendStr(&buf, "123");
			Assert::AreEqual("123", buf.data());

			buf.resize(0);
			InvestableIndex::Tools::appendStr(&buf, { "" });
			Assert::AreEqual("", buf.data());

			buf.resize(0);
			InvestableIndex::Tools::appendStr(&buf, { "", "", "" });
			Assert::AreEqual("", buf.data());

			InvestableIndex::Tools::appendStr(&buf, { "", "", "" });
			Assert::AreEqual("", buf.data());

			InvestableIndex::Tools::appendStr(&buf, { "A", "B", "C" });
			Assert::AreEqual("ABC", buf.data());

			InvestableIndex::Tools::appendStr(&buf, { "the", "quick", "brown", "fox", "jumps", "over", "the", "lazy", "dog." });
			Assert::AreEqual("ABCthequickbrownfoxjumpsoverthelazydog.", buf.data());
		}

		TEST_METHOD(AppendWStrUnitTest)
		{
			std::vector<wchar_t> buf;

			InvestableIndex::Tools::appendStr(&buf, L"");
			Assert::AreEqual(L"", buf.data());

			InvestableIndex::Tools::appendStr(&buf, L"");
			Assert::AreEqual(L"", buf.data());

			InvestableIndex::Tools::appendStr(&buf, L"123");
			Assert::AreEqual(L"123", buf.data());

			InvestableIndex::Tools::appendStr(&buf, L"123");
			Assert::AreEqual(L"123123", buf.data());

			buf.resize(0);
			InvestableIndex::Tools::appendStr(&buf, L"123");
			Assert::AreEqual(L"123", buf.data());

			buf.resize(0);
			InvestableIndex::Tools::appendStr(&buf, { L"" });
			Assert::AreEqual(L"", buf.data());

			buf.resize(0);
			InvestableIndex::Tools::appendStr(&buf, { L"", L"", L"" });
			Assert::AreEqual(L"", buf.data());

			InvestableIndex::Tools::appendStr(&buf, { L"", L"", L"" });
			Assert::AreEqual(L"", buf.data());

			InvestableIndex::Tools::appendStr(&buf, { L"A", L"B", L"C" });
			Assert::AreEqual(L"ABC", buf.data());
			
			InvestableIndex::Tools::appendStr(&buf, { L"the", L"quick", L"brown", L"fox", L"jumps", L"over", L"the", L"lazy", L"dog." });
			Assert::AreEqual(L"ABCthequickbrownfoxjumpsoverthelazydog.", buf.data());
		}
	};
	
	TEST_CLASS(StringFormatterUnitTest)
	{
	public:

		TEST_METHOD(ConvN2C)
		{
			InvestableIndex::Tools::StringFormatter<char> f;
			Assert::AreEqual("0", f.conv(0));
			Assert::AreEqual("1", f.conv(1));
			Assert::AreEqual("-1", f.conv(-1));
			Assert::AreEqual("10", f.conv(10));
			Assert::AreEqual("321", f.conv(321));
			Assert::AreEqual("123456789", f.conv(123456789));
			Assert::AreEqual("-123456789", f.conv(-123456789));
		}

		TEST_METHOD(ConvN2WC)
		{
			InvestableIndex::Tools::StringFormatter<wchar_t> f;
			Assert::AreEqual(L"0", f.conv(0));
			Assert::AreEqual(L"1", f.conv(1));
			Assert::AreEqual(L"-1", f.conv(-1));
			Assert::AreEqual(L"10", f.conv(10));
			Assert::AreEqual(L"321", f.conv(321));
			Assert::AreEqual(L"123456789", f.conv(123456789));
			Assert::AreEqual(L"-123456789", f.conv(-123456789));
		}
	
		TEST_METHOD(JoinStr)
		{
			InvestableIndex::Tools::StringFormatter<char> f;
			Assert::AreEqual("", f.join({ "" }));
			Assert::AreEqual("", f.join({ "", "" }));
			Assert::AreEqual("1", f.join({ "1" }));
			Assert::AreEqual("-1", f.join({ "-", "1" }));
			Assert::AreEqual("10", f.join({ "10", "" }));
			Assert::AreEqual("321", f.join({ "", "321" }));
			Assert::AreEqual("123456789", f.join({ "123", "456", "789" }));
			Assert::AreEqual("-123456789", f.join({ "-", "123", "456", "789" }));
			Assert::AreEqual("the quick brown fox jumps over the lazy dog.", 
				f.join({ "the", " ", "quick", " ", "brown", " ", "fox", " ", "jumps", " ",
					     "over", " ", "the", " ", "lazy", " ", "dog", "." }));
			Assert::AreEqual("------ ����������: ��Ŀ: InvestableIndexUnitTest, ����: Debug x64 ------\n"
							 "InvestableIndexBaseUnitTest.cpp\n"
							 "InvestableIndexUnitTest.vcxproj -> C:\\Users\\luojie\\source\\repos\\InvestableIndex\\x64\\Debug\\InvestableIndexUnitTest.dll\n"
							 "========== ����: �ɹ� 1 ����ʧ�� 0 �������� 2 �������� 0 �� ==========\n",
					f.join({ "------ ����������: ��Ŀ: InvestableIndexUnitTest, ����: Debug x64 ------", "\n",
							 "InvestableIndexBaseUnitTest.cpp", "\n",
							 "InvestableIndexUnitTest.vcxproj -> C:\\Users\\luojie\\source\\repos\\InvestableIndex\\x64\\Debug\\InvestableIndexUnitTest.dll", "\n",
							 "========== ����: �ɹ� 1 ����ʧ�� 0 �������� 2 �������� 0 �� ==========", "\n" }));
		}

		TEST_METHOD(JoinWStr)
		{
			InvestableIndex::Tools::StringFormatter<wchar_t> f;
			Assert::AreEqual(L"", f.join({ L"" }));
			Assert::AreEqual(L"", f.join({ L"", L"" }));
			Assert::AreEqual(L"1", f.join({ L"1" }));
			Assert::AreEqual(L"-1", f.join({ L"-", L"1" }));
			Assert::AreEqual(L"10", f.join({ L"10", L"" }));
			Assert::AreEqual(L"321", f.join({ L"", L"321" }));
			Assert::AreEqual(L"123456789", f.join({ L"123", L"456", L"789" }));
			Assert::AreEqual(L"-123456789", f.join({ L"-", L"123", L"456", L"789" }));
			Assert::AreEqual(L"the quick brown fox jumps over the lazy dog.",
					f.join({ L"the", L" ", L"quick", L" ", L"brown", L" ", L"fox", L" ",
							 L"jumps", L" ", L"over", L" ", L"the", L" ", L"lazy", L" ", L"dog", L"." }));
			Assert::AreEqual(L"------ ����������: ��Ŀ: InvestableIndexUnitTest, ����: Debug x64 ------\n"
							 L"InvestableIndexBaseUnitTest.cpp\n"
							 L"InvestableIndexUnitTest.vcxproj -> C:\\Users\\luojie\\source\\repos\\InvestableIndex\\x64\\Debug\\InvestableIndexUnitTest.dll\n"
							 L"========== ����: �ɹ� 1 ����ʧ�� 0 �������� 2 �������� 0 �� ==========\n",
					f.join({ L"------ ����������: ��Ŀ: InvestableIndexUnitTest, ����: Debug x64 ------", L"\n",
							 L"InvestableIndexBaseUnitTest.cpp", L"\n",
							 L"InvestableIndexUnitTest.vcxproj -> C:\\Users\\luojie\\source\\repos\\InvestableIndex\\x64\\Debug\\InvestableIndexUnitTest.dll", L"\n",
							 L"========== ����: �ɹ� 1 ����ʧ�� 0 �������� 2 �������� 0 �� ==========", L"\n" }));
		}

		TEST_METHOD(JoinInt)
		{
			InvestableIndex::Tools::StringFormatter<wchar_t> f;
			std::vector<int> data;
			Assert::AreEqual(L"", f.join(data));
			data.push_back(1);
			Assert::AreEqual(L"1", f.join(data));
			data.push_back(2);
			Assert::AreEqual(L"1,2", f.join(data));
			data.push_back(5);
			Assert::AreEqual(L"1,2,5", f.join(data));
			data.push_back(15);
			Assert::AreEqual(L"1,2,5,15", f.join(data));
		}

	};

}
