// InvestableIndex.cpp : 定义控制台应用程序的入口点。
//

#include "pch.h"

#include "IndexInterface.h"

using namespace std;

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
		cout << '>';
		cin.getline(cmdbuf, CMDBUFSIZE);

		// a d e f g h i j k l m n o p s t u v x y z
		// b设定指数基点 ps起始日期 pe截止日期 pt个股上限 pf固定更换日期
		// c备选池
		// r指数计算/计算结果
		// q退出 w保存
		switch (cmdbuf[0])
		{
		case 'q':
			cout << "退出" << endl;
			return 0;

		case 'b':
			index->setIndexBase(atoi(cmdbuf + 1));
			cout << "指数基准为" << atoi(cmdbuf + 1) << endl;
			break;

		case 'p':
			switch (cmdbuf[1]) {
			case 's':
				index->setPeriodStartDate(atoi(cmdbuf + 2));
				cout << "起始日期修改为" << atoi(cmdbuf + 2) << endl;
				break;

			case 'e':
				index->setPeriodEndDate(atoi(cmdbuf + 2));
				cout << "截止日期修改为" << atoi(cmdbuf + 2) << endl;
				break;

			case 'f':
				index->setFixedDate(atoi(cmdbuf + 2));
				cout << "更换成分股日期参数" << atoi(cmdbuf + 2) << endl;
				break;

			case 't':
				index->setTopStkWeight(atoi(cmdbuf + 2));
				cout << "weight factor " << (atoi(cmdbuf + 2) / 100.0) << endl;
				break;

			case 'c':
				index->setTopStkCount(atoi(cmdbuf + 2));
				cout << "stk cnt " << atoi(cmdbuf + 2) << endl;
				break;

			case 'w':
				int wt = atoi(cmdbuf + 2);
				cout << "wtype " << atoi(cmdbuf + 2) << endl;
				index->setWeightType(wt==1 ? WeightType::TOTALSHARE : WeightType::FREESHARE);
				break;
			}
			break;

		case 'c':
			if (cmdbuf[1] == '3') {
				cout << "switch c300" << endl;
				index->setIndexBasePool(3);
			}
			else if (cmdbuf[1] == '5') {
				cout << "switch c500" << endl;
				index->setIndexBasePool(5);
			}
			else if (cmdbuf[1] == '9') {
				cout << "switch c985" << endl;
				index->setIndexBasePool(9);
			}
			else if (cmdbuf[1] == '0') {
				cout << "delete base pool" << endl;
				index->setIndexBasePool(0);
			}
			else if (cmdbuf[1] == 's') {
				cout << "section " << atoi(cmdbuf + 2) << endl;
				index->appendSectionPool(atoi(cmdbuf + 2));
			}
			else if (cmdbuf[1] == 'l' && cmdbuf[2] == 'r') {
				cout << "read list" << endl;
				char filepath[MAX_PATH] = { 0 };
				if (getOpenFile(filepath, MAX_PATH)) {
					index->appendCSVPool(filepath);
				}
				else {
					cout << CommDlgExtendedError() << endl;
				}
			}
			else if (cmdbuf[1] == 'l' && cmdbuf[2] == '0') {
				cout << "clean list" << endl;
				index->removePools();
			}
			else if (cmdbuf[1] == 'b' && cmdbuf[2] == 'r') {
				cout << "switch file" << endl;
				//从文件读，未实现
				//index.setBasePool(idx985com);
			}
			break;

		case 'r':
			if (cmdbuf[1] == 'u' && cmdbuf[2] == 'n') {
				cout << "指数计算...";
				index->simulate();
				cout << "完成" << endl;
			}
			else if (cmdbuf[1] == 'w' && cmdbuf[2] == 's') {
				long long date = atoi(cmdbuf + 3);
				vector<long long> sect;
				vector<double> weight;
				index->getOpenSectionWeight(date, &sect, &weight, 1);
				auto weight_iter = weight.begin();
				for (auto iter = sect.begin(); iter != sect.end(); ++iter, ++weight_iter) {
					cout << *iter << ',' << *weight_iter << endl;
				}
			}
			else if (cmdbuf[1] == 'w') {
				long long date = atoi(cmdbuf + 2);
				vector<long long> stk;
				vector<double> weight;
				index->getOpenWeight(date, &stk, &weight);
				auto weight_iter = weight.begin();
				for (auto iter = stk.begin(); iter != stk.end(); ++iter, ++weight_iter) {
					cout << *iter << ',' << *weight_iter << endl;
				}
			}
			else if (cmdbuf[1] == 'e' && cmdbuf[2] == 't') {
				int cnt = index->getDayCount();
				const vector<double>& daily = index->getDailyReturn();
				const vector<double>& r = index->getCumReturn();
				for (int i = 0; i < cnt; i++) {
					cout << index->getDateByIndex(i) << ',' << daily[i + 1] << ',' << r[i + 1] << endl;
				}
			}
			else if (cmdbuf[1] == 'y') {
				double base = index->getIndexBase();
				int year = static_cast<int>(index->getDateByIndex(0)) / 10000;
				int cnt = index->getDayCount();
				const vector<double>& r = index->getCumReturn();
				for (int i = 0; i < cnt; i++) {
					if (i + 1 < cnt && index->getDateByIndex(i + 1) / 10000 == year) {
						continue;
					}
					cout << year << ',' << r[i + 1] / base - 1 << ',' << index->getDateByIndex(i) << ',' << r[i + 1] << endl;
					if (i + 1 < cnt) {
						year = static_cast<int>(index->getDateByIndex(i + 1)) / 10000;
						base = r[i + 1];
					}
				}
			}
			break;

		case 'w':
			if (cmdbuf[1] == 'r' && cmdbuf[2] == 'd') {
				char filepath[MAX_PATH] = { 0 };
				cout << "write seq" << endl;
				if (getSaveFile(filepath, MAX_PATH)) {
					index->writeFileReturnDaily(filepath);
				}
				else {
					cout << CommDlgExtendedError() << endl;
				}

			}
			else if (cmdbuf[1] == 'r' && cmdbuf[2] == 'y') {
				char filepath[MAX_PATH] = { 0 };
				cout << "write y seq" << endl;
				if (getSaveFile(filepath, MAX_PATH)) {
					index->writeFileReturnYearly(filepath);
				}
				else {
					cout << CommDlgExtendedError() << endl;
				}

			}
			else if (cmdbuf[1] == 'w') {
				char filepath[MAX_PATH] = { 0 };
				cout << "write weight" << endl;
				if (getSaveFile(filepath, MAX_PATH)) {
					index->writeFileWeightDay(filepath, atoi(cmdbuf + 2));
				}
				else {
					cout << CommDlgExtendedError() << endl;
				}

			}
			break;
		case 'T':
			index->test();
			break;

		case 'P':
			for (int i = 0; i < 200; ++i) {
				cout << "performance..." << i << endl;
				index->simulate();
			}
			cout << "退出" << endl;
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
		FILE* fp;
		_tfreopen_s(&fp, _T("CONOUT$"), _T("w"), stdout);
		_tfreopen_s(&fp, _T("CONIN$"), _T("r"), stdin);
		_tfreopen_s(&fp, _T("CONOUT$"), _T("w"), stderr);
		int rs = InvestableIndexCmdline();
		fclose(stderr);
		fclose(stdin);
		fclose(stdout);
		FreeConsole();
		return rs;
	}
	else {
		MessageBox(NULL, _T("没有命令行"), _T("IICore"), MB_OK);
		return 0;
	}
}
