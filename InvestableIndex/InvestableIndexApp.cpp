#include "framework.h"
#include "InvestableIndexApp.h"

InvestableIndexApp::InvestableIndexApp() : m_pDirect2dFactory(NULL)
{
}


InvestableIndexApp::~InvestableIndexApp()
{
}

void InvestableIndexApp::RunMessageLoop(concurrency::cancellation_token* token)
{
	MSG msg;

	while (::GetMessage(&msg, NULL, 0, 0))
	{
		::TranslateMessage(&msg);
		::DispatchMessage(&msg);
		if (token != nullptr && token->is_canceled()) {
			m_ChartWindow.Close();
		}
	}
}


HRESULT InvestableIndexApp::Initialize()
{
	HRESULT hr;

	// Initialize device-indpendent resources, such
	// as the Direct2D factory.
	hr = CreateDeviceIndependentResources();

	if (SUCCEEDED(hr))
	{
		hr = m_ChartWindow.Initialize(m_pDirect2dFactory);
	}

	return hr;
}


HRESULT InvestableIndexApp::CreateDeviceIndependentResources()
{
	HRESULT hr = S_OK;

	// Create a Direct2D factory.
	hr = ::D2D1CreateFactory(D2D1_FACTORY_TYPE_SINGLE_THREADED, &m_pDirect2dFactory);

	return hr;
}
