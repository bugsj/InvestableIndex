// InvestableIndex.cpp : 定义应用程序的入口点。
//

#include "framework.h"
#include <ppltasks.h>
#include <d2d1helper.h>

#include "InvestableIndex.h"
#include "InvestableIndexApp.h"

#include "../InvestableIndexCore/CUI.h"


int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
                      _In_opt_ HINSTANCE hPrevInstance,
                      _In_ LPWSTR    lpCmdLine,
                      _In_ int       nCmdShow)
{
    // TODO: 在此处放置代码。
	HeapSetInformation(NULL, HeapEnableTerminationOnCorruption, NULL, 0);

	HRESULT co_hr = CoInitialize(NULL);
	concurrency::cancellation_token_source cts;
	concurrency::task<void> chart_task;

	concurrency::cancellation_token token = cts.get_token();

	if (SUCCEEDED(co_hr)) {
		
		chart_task = concurrency::create_task([&] {
			InvestableIndexApp app;

			if (SUCCEEDED(app.Initialize()))
			{
				app.RunMessageLoop(&token);
			}
		}, token);
	}

	InvestableIndexCUI();

	if (SUCCEEDED(co_hr)) {
		cts.cancel();
		chart_task.wait();
		CoUninitialize();
	}

	return 0;
}
