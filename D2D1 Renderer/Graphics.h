#pragma once
#include <d2d1.h>
#include <windows.h>

class Graphics
{
	ID2D1Factory* factory;
	ID2D1SolidColorBrush* brush;
	ID2D1HwndRenderTarget* renderTarget;
public:
	Graphics();
	~Graphics();

	bool Init(HWND windowHandle);

	void ClearScreen(float r, float g, float b);
	void DrawCircle(float x, float y, float radius, float r, float g, float b, float a);
	void DrawRectangle(int x, int y, int width, int height, int linewidth, D2D1::ColorF color);
	void DrawRectangle(const D2D1_RECT_F& rect, ID2D1Brush* brush, FLOAT strokeWidth = 1.0f, ID2D1StrokeStyle* strokeStyle = NULL);
	void DrawRoundedRectangle(int x, int y, int width, int height, float radiusX, float radiusY, D2D1::ColorF color);

};