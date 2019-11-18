// IICoreTest.cpp : 此文件包含 "main" 函数。程序执行将在此处开始并结束。
//
#include <iostream>
#include <fstream>
#include <vector>
#include <map>
#include <unordered_map>
#include <set>
#include <memory>
#include <iomanip>
#include <algorithm>
#include <iterator>
#include <functional>
#include <numeric>

#include <Windows.h>
#include <stdlib.h>

using namespace std;

#include "../InvestableIndexCore/InterfaceC.h"
#pragma comment(lib,"InvestableIndexCore.lib")


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

int main()
{
	int indexobj;

	indexobj = IIInit();

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
			IISetIndexBase(indexobj, atoi(cmdbuf + 1));
			cout << "指数基准为" << atoi(cmdbuf + 1) << endl;
			break;

		case 'p':
			switch (cmdbuf[1]) {
			case 's':
				IISetStartDate(indexobj, atoi(cmdbuf + 2));
				cout << "起始日期修改为" << atoi(cmdbuf + 2) << endl;
				break;

			case 'e':
				IISetEndDate(indexobj, atoi(cmdbuf + 2));
				cout << "截止日期修改为" << atoi(cmdbuf + 2) << endl;
				break;

			case 'f':
				IISetFixDateFlag(indexobj, atoi(cmdbuf + 2));
				cout << "更换成分股日期参数" << atoi(cmdbuf + 2) << endl;
				break;

			case 't':
				IISetTopStkWeight(indexobj, atoi(cmdbuf + 2));
				cout << "weight factor " << (atoi(cmdbuf + 2) / 100.0) << endl;
				break;

			case 'c':
				IISetTopStkCount(indexobj, atoi(cmdbuf + 2));
				cout << "stk cnt " << atoi(cmdbuf + 2) << endl;
				break;
			}
			break;

		case 'c':
			if (cmdbuf[1] == '3') {
				cout << "switch c300" << endl;
				IISetIndexBasePool(indexobj, 3);
			}
			else if (cmdbuf[1] == '5') {
				cout << "switch c500" << endl;
				IISetIndexBasePool(indexobj, 5);
			}
			else if (cmdbuf[1] == '9') {
				cout << "switch c985" << endl;
				IISetIndexBasePool(indexobj, 9);
			}
			else if (cmdbuf[1] == '0') {
				cout << "delete base pool" << endl;
				IISetIndexBasePool(indexobj, 0);
			}
			else if (cmdbuf[1] == 'l' && cmdbuf[2] == 'r') {
				cout << "read list" << endl;
				char filepath[MAX_PATH] = { 0 };
				if (getOpenFile(filepath, MAX_PATH)) {
					IIAppendCSVPool(indexobj, filepath);
				}
				else {
					cout << CommDlgExtendedError() << endl;
				}
			}
			else if (cmdbuf[1] == 'l' && cmdbuf[2] == 'l') {
				cout << "input list" << endl;
				vector<int> stks;
				int stk;
				do {
					cout << ':';
					cin >> stk;
					if (stk > 0) {
						stks.push_back(stk);
					}
				} while (stk > 0);
				if (stks.size() > 0) {
					IIAppendSimplePool(indexobj, static_cast<int>(stks.size()), stks.data());
				}
				else {
					cout << "empty" << endl;
				}
			}
			else if (cmdbuf[1] == 'l' && cmdbuf[2] == '0') {
				cout << "clean list" << endl;
				IIRemovePools(indexobj);
			}
			else if (cmdbuf[1] == 'b' && cmdbuf[2] == 'r') {
				cout << "switch file" << endl;
				//从文件读，未实现
				//index.setBasePool(idx985com);
			}
			break;

		case 'r':
			if (cmdbuf[1] == 'u' && cmdbuf[2] == 'n') {
				int rs;
				cout << "指数计算...";
				rs = IISimulate(indexobj);
				if (rs == 0) {
					cout << "完成" << endl;
				}
				else if (rs == ERR_NOPOOLS) {
					cout << "无股票池" << endl;
				}
				else {
					cout << "错误" << endl;
				}
			}
			else if (cmdbuf[1] == 'w' && cmdbuf[2] == 's') {
				int date = atoi(cmdbuf + 3);
				int cnt;
				vector<int> sect;
				vector<double> weight;
				cnt = IIGetSecWeightCount(indexobj, date, 1);
				sect.resize(cnt);
				weight.resize(cnt);
				IIGetSecWeight(indexobj, date, sect.data(), weight.data(), 1);
				auto weight_iter = weight.begin();
				for (auto iter = sect.begin(); iter != sect.end(); ++iter, ++weight_iter) {
					cout << *iter << ',' << *weight_iter << endl;
				}
			}
			else if (cmdbuf[1] == 'w') {
				int date = atoi(cmdbuf + 2);
				int cnt;
				vector<int> stk;
				vector<double> weight;
				cnt = IIGetWeightCount(indexobj, date);
				stk.resize(cnt);
				weight.resize(cnt);
				IIGetWeight(indexobj, date, stk.data(), weight.data());
				auto weight_iter = weight.begin();
				for (auto iter = stk.begin(); iter != stk.end(); ++iter, ++weight_iter) {
					cout << *iter << ',' << *weight_iter << endl;
				}
			}
			else if (cmdbuf[1] == 'e' && cmdbuf[2] == 't') {
				int cnt = IIGetCumReturn(indexobj, nullptr);
				vector<double> daily(cnt);
				IIGetDailyReturn(indexobj, daily.data());
				vector<double> r(cnt);
				IIGetCumReturn(indexobj, r.data());
				for (int i = 0; i < cnt - 1; i++) {
					cout << IIGetDateByIndex(indexobj, i) << ',' << daily[i + 1] << ',' << r[i + 1] << endl;
				}
			}
			else if (cmdbuf[1] == 'y') {
				double base = IIGetIndexBase(indexobj);
				int year = IIGetDateByIndex(indexobj, 0) / 10000;
				int cnt = IIGetCumReturn(indexobj, nullptr);
				vector<double> r(cnt);
				IIGetCumReturn(indexobj, r.data());
				for (int i = 0; i < cnt - 1; i++) {
					if (i + 1 < cnt && IIGetDateByIndex(indexobj, i + 1) / 10000 == year) {
						continue;
					}
					cout << year << ',' << r[i + 1] / base - 1 << ',' << IIGetDateByIndex(indexobj, i) << ',' << r[i + 1] << endl;
					if (i + 1 < cnt) {
						year = IIGetDateByIndex(indexobj, i + 1) / 10000;
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
					IIWriteReturnDaily(indexobj, filepath);
				}
				else {
					cout << CommDlgExtendedError() << endl;
				}

			}
			else if (cmdbuf[1] == 'r' && cmdbuf[2] == 'y') {
				char filepath[MAX_PATH] = { 0 };
				cout << "write y seq" << endl;
				if (getSaveFile(filepath, MAX_PATH)) {
					IIWriteReturnYearly(indexobj, filepath);
				}
				else {
					cout << CommDlgExtendedError() << endl;
				}

			}
			else if (cmdbuf[1] == 'w') {
				char filepath[MAX_PATH] = { 0 };
				cout << "write weight" << endl;
				if (getSaveFile(filepath, MAX_PATH)) {
					IIWriteWeightDay(indexobj, filepath, atoi(cmdbuf + 2));
				}
				else {
					cout << CommDlgExtendedError() << endl;
				}

			}
			break;
		case 'T':
			//index.test();
			break;

		case 'P':
			int rs;
			IISetIndexBasePool(indexobj, 3);
			//IISetSectionPool(indexobj, 196879);
			for (int i = 0; i < 200; ++i) {
				cout << "performance..." << i << endl;
				rs = IISimulate(indexobj);
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

