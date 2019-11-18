#pragma once
class ChartWindow
{
public:
	ChartWindow();
	~ChartWindow();

	HRESULT Initialize(ID2D1Factory* Direct2dFactory);
	BOOL Close() { return PostMessage(m_hwnd, WM_CLOSE, 0, 0); }

private:
	HRESULT SetDeviceIndependentResources(ID2D1Factory* Direct2dFactory);
	HRESULT CreateDeviceResources();
	void DiscardDeviceResources();
	HRESULT OnRender();
	void OnResize(UINT width, UINT height);
	static LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);

private:
	HWND m_hwnd;
	ID2D1Factory* m_pDirect2dFactory;
	ID2D1HwndRenderTarget* m_pRenderTarget;
	ID2D1SolidColorBrush* m_pLightSlateGrayBrush;
	ID2D1SolidColorBrush* m_pCornflowerBlueBrush;
};

