#include "graphics.h"


Graphics::Graphics()
{
	factory = NULL;
	renderTarget = NULL;
	brush = NULL;
	dwriteFactory = NULL;
	textFormat = NULL;

	if (dwriteFactory) {
		dwriteFactory->CreateTextFormat(
			L"Arial",
			NULL,
			DWRITE_FONT_WEIGHT_NORMAL,
			DWRITE_FONT_STYLE_NORMAL,
			DWRITE_FONT_STRETCH_NORMAL,
			20.f,
			L"",
			&textFormat
		);
}
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

	// Initialize DirectWrite Factory
	res = DWriteCreateFactory(DWRITE_FACTORY_TYPE_SHARED, __uuidof(dwriteFactory), reinterpret_cast<IUnknown**>(&dwriteFactory));
	if (res != S_OK) return false;

	RECT rect;
	GetClientRect(windowHandle, &rect);

	res = factory->CreateHwndRenderTarget(
		D2D1::RenderTargetProperties(),
		D2D1::HwndRenderTargetProperties(
			windowHandle, D2D1::SizeU(rect.right, rect.bottom)),
		&renderTarget);

	// Initialize your text format here
	if (dwriteFactory) {
		dwriteFactory->CreateTextFormat(
			L"Arial",  // Font family
			NULL,  // Font collection (use NULL for the system font collection)
			DWRITE_FONT_WEIGHT_NORMAL,
			DWRITE_FONT_STYLE_NORMAL,
			DWRITE_FONT_STRETCH_NORMAL,
			16.0f,  // Font size
			L"",  // Locale
			&textFormat
		);
	}

	// ... Rest of your existing initialization code ...

	return true;
}




void Graphics::ClearScreen(float r, float g, float b)

{
	renderTarget->Clear(D2D1::ColorF(r, g, b));
}

void Graphics::DrawCircle(float x, float y, float radius, float r, float g, float b, float a)
{
	renderTarget->CreateSolidColorBrush(D2D1::ColorF(r, g, b, a), &brush);

	renderTarget->DrawEllipse(D2D1::Ellipse(D2D1::Point2F(x, y), radius, radius), brush, 3.0f);  //circle is just an ellipse with the two radius' being the same

}
void Graphics::DrawRectangle(int x, int y, int width, int height, int linewidth, D2D1::ColorF colour)
{
	renderTarget->CreateSolidColorBrush(D2D1::ColorF(0.0f, 0.0f, 0.0f, 1.0f), &brush);

	D2D1_RECT_F rect = { static_cast<float>(x), static_cast<float>(y), static_cast<float>(width + x), static_cast<float>(height + y) };

	renderTarget->DrawRectangle(rect, brush, linewidth);
}
void Graphics::DrawRoundedRectangle(int x, int y, int width, int height, float radiusX, float radiusY, D2D1::ColorF color)
{
	renderTarget->CreateSolidColorBrush(color, &brush);

	D2D1_ROUNDED_RECT roundedRect = D2D1::RoundedRect(D2D1::RectF(20.f, 20.f, 150.f, 100.f),10.f,10.f);

	renderTarget->DrawRoundedRectangle(roundedRect, brush, 3.0f);
}

template <class T>
void SafeRelease(T** ppT)
{
	if (*ppT)
	{
		(*ppT)->Release();
		*ppT = NULL;
	}
}

void Graphics::DrawTriangle(float x1, float y1, float x2, float y2, float x3, float y3, D2D1::ColorF color)
{
	renderTarget->CreateSolidColorBrush(color, &brush);

	// Define the triangle's vertices
	D2D1_POINT_2F points[] = {
		D2D1::Point2F(x1, y1),
		D2D1::Point2F(x2, y2),
		D2D1::Point2F(x3, y3)
	};

	// Create a path geometry and add the triangle as a path
	ID2D1PathGeometry* pathGeometry = NULL;
	factory->CreatePathGeometry(&pathGeometry);

	ID2D1GeometrySink* sink = NULL;
	pathGeometry->Open(&sink);

	sink->BeginFigure(points[0], D2D1_FIGURE_BEGIN_FILLED);
	sink->AddLine(points[1]);
	sink->AddLine(points[2]);
	sink->EndFigure(D2D1_FIGURE_END_CLOSED);
	sink->Close();

	// Draw the triangle geometry
	renderTarget->FillGeometry(pathGeometry, brush);

	// Release resources
	SafeRelease(&pathGeometry);
}
void Graphics::DrawSemiCircle(float x, float y, float radius, D2D1::ColorF color)
{
	renderTarget->CreateSolidColorBrush(color, &brush);

	D2D1_ELLIPSE ellipse = D2D1::Ellipse(D2D1::Point2F(x, y), radius, radius);


	float startAngle = 0.0f;
	float sweepAngle = 180.0f;

	ID2D1PathGeometry* pathGeometry = NULL;
	factory->CreatePathGeometry(&pathGeometry);

	ID2D1GeometrySink* sink = NULL;
	pathGeometry->Open(&sink);

	sink->SetFillMode(D2D1_FILL_MODE_WINDING);

	D2D1_POINT_2F startPoint = D2D1::Point2F(x, y - radius);

	sink->BeginFigure(startPoint, D2D1_FIGURE_BEGIN_FILLED);


	D2D1_ARC_SEGMENT arc;
	arc.point = D2D1::Point2F(x, y - radius);
	arc.size = D2D1::SizeF(radius, radius);
	arc.rotationAngle = 0.0f;
	arc.sweepDirection = D2D1_SWEEP_DIRECTION_CLOCKWISE;
	arc.arcSize = D2D1_ARC_SIZE_SMALL;

	sink->AddArc(arc);

	sink->EndFigure(D2D1_FIGURE_END_CLOSED);

	sink->Close();

	renderTarget->DrawGeometry(pathGeometry, brush, 3.0f);

	SafeRelease(&pathGeometry);
	SafeRelease(&sink);
}
void Graphics::FillRectangle(int x, int y, int width, int height, D2D1::ColorF color)
{
	renderTarget->CreateSolidColorBrush(color, &brush);

	D2D1_RECT_F rect = { static_cast<float>(x), static_cast<float>(y), static_cast<float>(width + x), static_cast<float>(height + y) };
	renderTarget->FillRectangle(rect, brush);
}
void Graphics::FillRoundedRectangle(int x, int y, int width, int height, float radiusX, float radiusY, D2D1::ColorF color)
{
	renderTarget->CreateSolidColorBrush(color, &brush);

	D2D1_ROUNDED_RECT roundedRect = D2D1::RoundedRect(D2D1::RectF(static_cast<float>(x), static_cast<float>(y), static_cast<float>(width + x), static_cast<float>(height + y)), radiusX, radiusY);
	renderTarget->FillRoundedRectangle(roundedRect, brush);
}

void Graphics::DrawText(const WCHAR* text, const D2D1_RECT_F& layoutRect, D2D1::ColorF textColor, IDWriteTextFormat* textFormat)
{
	renderTarget->CreateSolidColorBrush(textColor, &brush);

	renderTarget->DrawText(
		text,
		wcslen(text),
		textFormat,  // Specify the text format
		layoutRect,
		brush
	);

	SafeRelease(&brush);
}


