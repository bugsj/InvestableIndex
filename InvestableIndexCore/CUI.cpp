// InvestableIndex.cpp : 定义控制台应用程序的入口点。
//

#include "pch.h"

#include "IndexInterface.h"

namespace InvestableIndex {

	BOOL getSaveFile(char* filename, int maxfile)
	{
		OPENFILENAMEA ofn;
		memset(&ofn, 0, sizeof(ofn));
		ofn.lStructSize = sizeof(ofn);
		ofn.nMaxFile = maxfile;
		ofn.lpstrFilter = "数据文件(*.csv)\0*.csv\0所有文件(*.*)\0*.*\0\0";
		ofn.lpstrFile = filename;
		ofn.lpstrDefExt = "csv";
		ofn.nFilterIndex = 1;
		ofn.Flags = OFN_CREATEPROMPT | OFN_OVERWRITEPROMPT;

		return GetSaveFileNameA(&ofn);
	}

	BOOL getOpenFile(char* filename, int maxfile)
	{
		OPENFILENAMEA ofn;
		memset(&ofn, 0, sizeof(ofn));
		ofn.lStructSize = sizeof(ofn);
		ofn.nMaxFile = maxfile;
		ofn.lpstrFilter = "数据文件(*.csv)\0*.csv\0所有文件(*.*)\0*.*\0\0";
		ofn.lpstrFile = filename;
		ofn.lpstrDefExt = "csv";
		ofn.nFilterIndex = 1;
		ofn.Flags = OFN_DONTADDTORECENT | OFN_FILEMUSTEXIST;

		return GetOpenFileNameA(&ofn);
	}

	int InvestableIndexCmdline()
	{
		IndexInterface* index = IndexInterface::get();

		const int CMDBUFSIZE = 256;
		char cmdbuf[CMDBUFSIZE];
		while (true) {
			std::cout << '>';
			std::cin.getline(cmdbuf, CMDBUFSIZE);

			// a d e f g h i j k l m n o p s t u v x y z
			// b设定指数基点 ps起始日期 pe截止日期 pt个股上限 pf固定更换日期
			// c备选池
			// r指数计算/计算结果
			// q退出 w保存
			switch (cmdbuf[0])
			{
			case 'q':
				std::cout << "退出" << std::endl;
				return 0;

			case 'b':
				index->setIndexBase(atoi(cmdbuf + 1));
				std::cout << "指数基准为" << atoi(cmdbuf + 1) << std::endl;
				break;

			case 'p':
				switch (cmdbuf[1]) {
				case 's':
					index->setPeriodStartDate(atoi(cmdbuf + 2));
					std::cout << "起始日期修改为" << atoi(cmdbuf + 2) << std::endl;
					break;

				case 'e':
					index->setPeriodEndDate(atoi(cmdbuf + 2));
					std::cout << "截止日期修改为" << atoi(cmdbuf + 2) << std::endl;
					break;

				case 'f':
					index->setFixedDate(atoi(cmdbuf + 2));
					std::cout << "更换成分股日期参数" << atoi(cmdbuf + 2) << std::endl;
					break;

				case 't':
					index->setTopStkWeight(atoi(cmdbuf + 2));
					std::cout << "weight factor " << (atoi(cmdbuf + 2) / 100.0) << std::endl;
					break;

				case 'c':
					index->setTopStkCount(atoi(cmdbuf + 2));
					std::cout << "stk cnt " << atoi(cmdbuf + 2) << std::endl;
					break;

				case 'w':
					int wt = atoi(cmdbuf + 2);
					std::cout << "wtype " << atoi(cmdbuf + 2) << std::endl;
					index->setWeightType(wt == 1 ? WeightType::TOTALSHARE : WeightType::FREESHARE);
					break;
				}
				break;

			case 'c':
				if (cmdbuf[1] == '3') {
					std::cout << "switch c300" << std::endl;
					index->setIndexBasePool(3);
				}
				else if (cmdbuf[1] == '5') {
					std::cout << "switch c500" << std::endl;
					index->setIndexBasePool(5);
				}
				else if (cmdbuf[1] == '9') {
					std::cout << "switch c985" << std::endl;
					index->setIndexBasePool(9);
				}
				else if (cmdbuf[1] == '0') {
					std::cout << "delete base pool" << std::endl;
					index->setIndexBasePool(0);
				}
				else if (cmdbuf[1] == 's') {
					std::cout << "section " << atoi(cmdbuf + 2) << std::endl;
					index->appendSectionPool(atoi(cmdbuf + 2));
				}
				else if (cmdbuf[1] == 'l' && cmdbuf[2] == 'r') {
					std::cout << "read list" << std::endl;
					char filepath[MAX_PATH] = { 0 };
					if (getOpenFile(filepath, MAX_PATH)) {
						index->appendCSVPool(filepath);
					}
					else {
						std::cout << CommDlgExtendedError() << std::endl;
					}
				}
				else if (cmdbuf[1] == 'l' && cmdbuf[2] == '0') {
					std::cout << "clean list" << std::endl;
					index->removePools();
				}
				else if (cmdbuf[1] == 'b' && cmdbuf[2] == 'r') {
					std::cout << "switch file" << std::endl;
					//从文件读，未实现
					//index.setBasePool(idx985com);
				}
				break;

			case 'r':
				if (cmdbuf[1] == 'u' && cmdbuf[2] == 'n') {
					std::cout << "指数计算...";
					index->simulate();
					std::cout << "完成" << std::endl;
				}
				else if (cmdbuf[1] == 'w' && cmdbuf[2] == 's') {
					long long date = atoi(cmdbuf + 3);
					std::vector<long long> sect;
					std::vector<double> weight;
					index->getOpenSectionWeight(date, &sect, &weight, 1);
					auto weight_iter = weight.cbegin();
					for (auto iter = sect.cbegin(); iter != sect.cend(); ++iter, ++weight_iter) {
						std::cout << *iter << ',' << *weight_iter << std::endl;
					}
				}
				else if (cmdbuf[1] == 'w') {
					long long date = atoi(cmdbuf + 2);
					std::vector<long long> stk;
					std::vector<double> weight;
					index->getOpenWeight(date, &stk, &weight);
					auto weight_iter = weight.cbegin();
					for (auto iter = stk.cbegin(); iter != stk.cend(); ++iter, ++weight_iter) {
						std::cout << *iter << ',' << *weight_iter << std::endl;
					}
				}
				else if (cmdbuf[1] == 'e' && cmdbuf[2] == 't') {
					long long cnt = index->getDayCount();
					const std::vector<double>& daily = index->getDailyReturn();
					const std::vector<double>& r = index->getCumReturn();
					for (long long i = 0; i < cnt; i++) {
						std::cout << index->getDateByIndex(i) << ',' << daily[i + 1] << ',' << r[i + 1] << std::endl;
					}
				}
				else if (cmdbuf[1] == 'y') {
					double base = static_cast<double>(index->getIndexBase());
					long long year = index->getDateByIndex(0) / 10000;
					long long cnt = index->getDayCount();
					const std::vector<double>& r = index->getCumReturn();
					for (int i = 0; i < cnt; i++) {
						if (i + 1 < cnt && index->getDateByIndex(i + 1) / 10000 == year) {
							continue;
						}
						std::cout << year << ',' << r[i + 1] / base - 1 << ',' << index->getDateByIndex(i) << ',' << r[i + 1] << std::endl;
						if (i + 1 < cnt) {
							year = index->getDateByIndex(i + 1) / 10000;
							base = r[i + 1];
						}
					}
				}
				break;

			case 'w':
				if (cmdbuf[1] == 'r' && cmdbuf[2] == 'd') {
					char filepath[MAX_PATH] = { 0 };
					std::cout << "write seq" << std::endl;
					if (getSaveFile(filepath, MAX_PATH)) {
						index->writeFileReturnDaily(filepath);
					}
					else {
						std::cout << CommDlgExtendedError() << std::endl;
					}

				}
				else if (cmdbuf[1] == 'r' && cmdbuf[2] == 'y') {
					char filepath[MAX_PATH] = { 0 };
					std::cout << "write y seq" << std::endl;
					if (getSaveFile(filepath, MAX_PATH)) {
						index->writeFileReturnYearly(filepath);
					}
					else {
						std::cout << CommDlgExtendedError() << std::endl;
					}

				}
				else if (cmdbuf[1] == 'w') {
					char filepath[MAX_PATH] = { 0 };
					std::cout << "write weight" << std::endl;
					if (getSaveFile(filepath, MAX_PATH)) {
						index->writeFileWeightDay(filepath, atoi(cmdbuf + 2));
					}
					else {
						std::cout << CommDlgExtendedError() << std::endl;
					}

				}
				break;
			case 'T':
				index->test();
				break;

			case 'P':
				for (int i = 0; i < 200; ++i) {
					std::cout << "performance..." << i << std::endl;
					index->simulate();
				}
				std::cout << "退出" << std::endl;
				return 0;
				break;

			default:
				break;
			}

		}

		return 0;
	}


	int InvestableIndexCUI()
	{
		if (AllocConsole()) {
			::FILE* fp;
			::_tfreopen_s(&fp, _T("CONOUT$"), _T("w"), stdout);
			::_tfreopen_s(&fp, _T("CONIN$"), _T("r"), stdin);
			::_tfreopen_s(&fp, _T("CONOUT$"), _T("w"), stderr);
			int rs = InvestableIndexCmdline();
			::fclose(stderr);
			::fclose(stdin);
			::fclose(stdout);
			::FreeConsole();
			return rs;
		}
		else {
			::MessageBox(NULL, _T("没有命令行"), _T("IICore"), MB_OK);
			return 0;
		}
	}

}
