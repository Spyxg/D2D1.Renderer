#pragma once
#include <d2d1.h>
#include <windows.h>
#include <dwrite.h>

class Graphics
{
	ID2D1Factory* factory;
	ID2D1SolidColorBrush* brush;
	ID2D1HwndRenderTarget* renderTarget;
	IDWriteFactory* dwriteFactory;  // DirectWrite Factory
	IDWriteTextFormat* textFormat;  // Text format

	float centerX;
	float centerY;
	float circleRadius;
	

public:
	Graphics();
	~Graphics();

	bool Init(HWND windowHandle);

	void BeginDraw() { renderTarget->BeginDraw(); }
	void EndDraw() { renderTarget->EndDraw(); }

	void ClearScreen(float r, float g, float b);
	void DrawCircle(float x, float y, float radius, float r, float g, float b, float a);
	void DrawRectangle(int x, int y, int width, int height, int linewidth, D2D1::ColorF color);
	void DrawRectangle(const D2D1_RECT_F& rect, ID2D1Brush* brush, FLOAT strokeWidth = 1.0f, ID2D1StrokeStyle* strokeStyle = NULL);
	void DrawRoundedRectangle(int x, int y, int width, int height, float radiusX, float radiusY, D2D1::ColorF color);
	void DrawTriangle(float x1, float y1, float x2, float y2, float x3, float y3, D2D1::ColorF color);
	void SetPosition(float x, float y, float radius);
	void DrawSemiCircle(float x, float y, float radius, D2D1::ColorF color);
	void FillRoundedRectangle(int x, int y, int width, int height, float radiusX, float radiusY, D2D1::ColorF color);
	void FillRectangle(int x, int y, int width, int height, D2D1::ColorF color);

	void DrawText(const WCHAR* text, const D2D1_RECT_F& layoutRect, D2D1::ColorF textColor);

};