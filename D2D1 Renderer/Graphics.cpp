#include "graphics.h"

Graphics::Graphics()
{
	factory = NULL;
	renderTarget = NULL;
	brush = NULL;
}
Graphics::~Graphics()
{
	if (factory) factory->Release(); //this releases all com interfaces
	if (renderTarget) renderTarget->Release();
	if (brush) brush->Release();
}

bool Graphics::Init(HWND windowHandle)
{
	HRESULT res = D2D1CreateFactory(D2D1_FACTORY_TYPE_SINGLE_THREADED, &factory);
	if (res != S_OK) return false;

	RECT rect;
	GetClientRect(windowHandle, &rect);

	res = factory->CreateHwndRenderTarget(
		D2D1::RenderTargetProperties(),
		D2D1::HwndRenderTargetProperties(
			windowHandle, D2D1::SizeU(rect.right, rect.bottom)),    // we essentially created a direct2d factory, to create the rendertarget and set it to the client area on our window
		&renderTarget);

	if (res != S_OK) return false;

	res = renderTarget->CreateSolidColorBrush(D2D1::ColorF(0, 0, 0, 0), &brush);  // Call render targets CreateSolidColorBrush method, this method takes a colorf parameter (())
	// and a pointer (->) to the ID2D1::SolidColorBrush you want to create		
	if (res != S_OK) return false;

	return true;
}

void Graphics::ClearScreen(float r, float g, float b)

{
	renderTarget->Clear(D2D1::ColorF(r, g, b));
}