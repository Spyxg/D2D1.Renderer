#include <windows.h>
#include "Graphics.h"

Graphics* graphics;

LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	if (uMsg == WM_DESTROY) { PostQuitMessage(0); return 0; }

	return DefWindowProc(hwnd, uMsg, wParam, lParam);
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
    windowclass.style = CS_HREDRAW | CS_VREDRAW;

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

    ShowWindow(windowhandle, nCmdShow);

    MSG message;
    message.message = WM_NULL;

    while (message.message != WM_QUIT)
    {
        if (PeekMessage(&message, NULL, 0, 0, PM_REMOVE))
            DispatchMessage(&message);
        else
        {
        //update


        //render
            graphics->BeginDraw();
            graphics->ClearScreen(0.0f, 0.0f, 0.5);

            graphics->DrawRoundedRectangle(500, 500, 500, 500, 125, 125, 1.0f);

            D2D1::ColorF semiCircleColor(0.0f, 0.0f, 0.0f, 1.0f);
            graphics->DrawSemiCircle(500, 500, 100, semiCircleColor);

            D2D1::ColorF triangleColor(0.0f, 0.0f, 0.0f, 1.0f);  // Red color (R, G, B, A)
            graphics->DrawTriangle(0.0f, 0.0f, 100.0f, 0.0f, 50.0f, 100.0f, triangleColor);

            
            graphics->EndDraw();
        }
    }


    delete graphics;

    return 0;
}
