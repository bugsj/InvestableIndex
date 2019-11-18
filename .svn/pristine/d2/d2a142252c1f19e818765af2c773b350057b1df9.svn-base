#pragma once

#include "ChartWindow.h"
#include "ppltasks.h"

class InvestableIndexApp
{
public:
	InvestableIndexApp();
	~InvestableIndexApp();

	HRESULT Initialize();
	void RunMessageLoop(concurrency::cancellation_token *token = nullptr);

private:
	HRESULT CreateDeviceIndependentResources();

private:
	ID2D1Factory* m_pDirect2dFactory;

	ChartWindow m_ChartWindow;
};
