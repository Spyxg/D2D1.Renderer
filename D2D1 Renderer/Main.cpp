#include <windows.h>
#include "Graphics.h"
#include <dwrite.h>
#include <map>
#include <string>

enum MouseAction {
    LeftMouseClick,
    RightMouseClick,
    MiddleMouseClick,
};

void HandleMouseAction(MouseAction action) {
    switch (action) {
    case LeftMouseClick:
        // Handle left mouse click action
        // Add your code here
        break;
    case RightMouseClick:
        // Handle right mouse click action
        // Add your code here
        break;
    case MiddleMouseClick:
        // Handle middle mouse click action
        // Add your code here
        break;
        // Add more cases for additional mouse actions as needed
    default:

        break;
    }
}

bool IsEnterKeyPressed()
{

    return GetAsyncKeyState(VK_RETURN) & 0x8000;
}

Graphics* graphics;

std::map<int, int> keyMap;


LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    if (uMsg == WM_DESTROY) {
        PostQuitMessage(0);
        return 0;
    }

    if (uMsg == WM_KEYDOWN) {
        int key = static_cast<int>(wParam);
        if (keyMap[key] == 0) {
            keyMap[key] = 1; // Key is clicked
        }
        else if (keyMap[key] == 1) {
            keyMap[key] = 2; // Key is down
        }
       else if (uMsg == WM_LBUTTONDOWN) {
 
        HandleMouseAction(LeftMouseClick);
        }
        else if (uMsg == WM_RBUTTONDOWN) {

        HandleMouseAction(RightMouseClick);
        }
        else if (uMsg == WM_MBUTTONDOWN) {

        HandleMouseAction(MiddleMouseClick);
        }

    }
    else if (uMsg == WM_KEYUP) {
        int key = static_cast<int>(wParam);
        keyMap[key] = 0; 
    }

    return DefWindowProc(hwnd, uMsg, wParam, lParam);
}

std::map<std::wstring, IDWriteTextFormat*> fontMap;

void CreateFontAndAddToMap(IDWriteFactory* dwFactory, const std::wstring& fontName, IDWriteTextFormat** font)
{
    dwFactory->CreateTextFormat(fontName.c_str(), NULL, DWRITE_FONT_WEIGHT_NORMAL, DWRITE_FONT_STYLE_NORMAL, DWRITE_FONT_STRETCH_NORMAL, 50.0f, L"", font);

}

int wWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPWSTR lpCmdLine, int nCmdShow)
{
    WNDCLASSEX windowclass;
    ZeroMemory(&windowclass, sizeof(WNDCLASSEX));
    windowclass.cbSize = sizeof(WNDCLASSEX);
    windowclass.hbrBackground = (HBRUSH)COLOR_WINDOW;  //background color of window
    windowclass.hInstance = hInstance;  // hInstance is typically passed to your WinMain or DllMain function.
    windowclass.lpfnWndProc = WindowProc;  //event handling procedure - Must add this for the procedure at the top
    windowclass.lpszClassName = L"MainWindow";
    windowclass.style = CS_HREDRAW | CS_VREDRAW;\
    
    std::map<int, int> keyMap; // Create the key state map

    RegisterClassEx(&windowclass); //& = reference to class

    RECT rect = { 0, 0, 800,600 }; //this will determine the size of the actual client window size, not including border, clickable, usable space.
    AdjustWindowRectEx(&rect, WS_OVERLAPPEDWINDOW, false, WS_EX_OVERLAPPEDWINDOW); //these are the parameters, if you need parameters, visit 

    HWND windowhandle = CreateWindowEx(WS_EX_OVERLAPPEDWINDOW, L"MainWindow", L"Shapes Test", WS_OVERLAPPEDWINDOW, 100, 100,
        rect.right - rect.left, rect.bottom - rect.top, NULL, NULL, hInstance, 0); // if you added rect then you will need to change the base windowhandle x and y values

    if (!windowhandle) return -1;

    graphics = new Graphics();

    if (!graphics->Init(windowhandle)) // ! means not, so if not graphics init then show error
    {
        delete graphics;
        return -1;
    }

    IDWriteFactory* dwFactory = NULL;

    ShowWindow(windowhandle, nCmdShow);
    HRESULT hr = DWriteCreateFactory(DWRITE_FACTORY_TYPE_SHARED, __uuidof(IDWriteFactory), reinterpret_cast<IUnknown**>(&dwFactory));

    CreateFontAndAddToMap(dwFactory, L"Arial", &fontMap[L"Arial"]);
    CreateFontAndAddToMap(dwFactory, L"Times New Roman", &fontMap[L"Times New Roman"]);
    CreateFontAndAddToMap(dwFactory, L"Calibri", &fontMap[L"Calibri"]);
    CreateFontAndAddToMap(dwFactory, L"Comic Sans", &fontMap[L"Comic Sans"]);
    CreateFontAndAddToMap(dwFactory, L"Tahoma", &fontMap[L"Tahoma"]);
    CreateFontAndAddToMap(dwFactory, L"Lato", &fontMap[L"Lato"]);
    CreateFontAndAddToMap(dwFactory, L"Consolas", &fontMap[L"Consolas"]);



    MSG message;
    message.message = WM_NULL;

    while (message.message != WM_QUIT)
    {
        if (PeekMessage(&message, NULL, 0, 0, PM_REMOVE))
        {
            DispatchMessage(&message);
        }
        else
        {
            if (IsEnterKeyPressed())
            {

                float x = 400.0f;  // Modify these values as needed
                float y = 300.0f;
                float radius = 50.0f;
                float r = 1.0f;
                float g = 0.0f;
                float b = 0.0f;
                float a = 1.0f;


                //update
               // const WCHAR* textToRender = L"JoJo has a small sack";
               // D2D1_RECT_F layoutRect = D2D1::RectF(100, 200, 600, 400.0f);
               // D2D1::ColorF textColor = D2D1::ColorF(0.0f, 0.0f, 0.0f);

                //render
                graphics->BeginDraw();
                graphics->ClearScreen(1.0f, 1.0f, 1.0);
                // IDWriteTextFormat* fontToUse = fontMap[L"Tahoma"];
                // graphics->DrawText(textToRender, layoutRect, textColor, fontToUse);
                graphics->DrawCircle(x, y, radius, r, g, b, a);
                graphics->EndDraw();
            }
        }
    }
    for (const auto& fontPair : fontMap)
    {

        fontPair.second->Release();
    }

    if (dwFactory)
    {

        dwFactory->Release();
    }



    delete graphics;

    return 0;
}
