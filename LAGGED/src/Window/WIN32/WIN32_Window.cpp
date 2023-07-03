#include "Precomp.h"
#include "WIN32_Window.h"

#include "Window/WIN32/WIN32_WindowClass.h"
#include "Window/WIN32/Exceptions/WIN32_ExceptionMacros.h"

#include "Events/WindowEvents.h"
#include "Utility/Logger.h"

#include "Platform/RendererBase.h"

namespace LAG::Window
{
	WIN32Data* winData = nullptr;

	static LRESULT HandleWindowProcedure(HWND, UINT, WPARAM, LPARAM);

	void Initialize(unsigned int winWidth, unsigned int winHeight, bool fullscreen, bool useVSync, bool centerWindow)
	{
		if (winData == nullptr)
		{
			winData = new WIN32Data();
		}
		else LAG::Utility::Logger::Error("WinData already exists.");

		try {
			//First, ensure that the window class has been set up.
			WindowClass::GetWinClass().SetupWindowClass(HandleWindowProcedure);

			winData->windowWidth = winWidth;
			winData->windowHeight = winHeight;
			winData->useVSync = useVSync;
			winData->isFullscreen = fullscreen;

			//Ensure that we don't get weird looking windows
			if (winData->windowWidth <= 0)
				winData->windowWidth = 800;
			if (winData->windowHeight <= 0)
				winData->windowHeight = 600;

			winData->windowWidth = winWidth;
			winData->windowHeight = winHeight;

			int winXPos = 10, winYPos = 10;
			if (centerWindow)
			{
				winXPos = (GetSystemMetrics(SM_CXSCREEN) - winData->windowWidth) / 2;
				winYPos = (GetSystemMetrics(SM_CYSCREEN) - winData->windowHeight) / 2;
			}


			//Set the right window size. WE DONT want to include the top part of the window (the non-client area), so we get rid of that using the size function below. 
			RECT winRect = { 0, 0, static_cast<LONG>(winData->windowWidth), static_cast<LONG>(winData->windowHeight) };
			AdjustWindowRect(&winRect, WS_CAPTION | WS_MINIMIZEBOX | WS_SYSMENU, FALSE);

			winData->hWnd = CreateWindow(
				WindowClass::GetWinClass().GetWindowClassName(), winData->windowName,
				WS_OVERLAPPEDWINDOW,
				winXPos, winYPos, winRect.right - winRect.left, winRect.bottom - winRect.top,
				nullptr, nullptr, WindowClass::GetWinClass().GetHInstance(), winData
			);

			//Check if the hwnd has been created properly. 
			if (winData->hWnd == NULL) {
				LAG_WINDOW_EXCEPTION_PREV();
			}

			//This could potentially end badly...! Too bad!
			ShowWindow(winData->hWnd, SW_SHOW);

			SetWindowLongPtr(winData->hWnd, GWLP_USERDATA, reinterpret_cast<LONG_PTR>(&winData->winEventCallback));
		}
		catch (WindowException& ex)
		{
			std::string exceptionDesc = ex.GetExceptionMessage();
			MessageBox(nullptr, exceptionDesc.c_str(), "Window Exception thrown!", MB_OK | MB_ICONEXCLAMATION);
		}
		catch (std::exception& ex)
		{
			MessageBox(nullptr, ex.what(), "Standard Exception thrown!", MB_OK | MB_ICONEXCLAMATION);
		}
	}

	void Shutdown()
	{
		if (winData != nullptr)
		{
			DestroyWindow(winData->hWnd);
			delete winData;
		}
		winData = nullptr;

		WindowClass::GetWinClass().Shutdown();
	}

	bool HandleWindowMessages(int& exitCodeOut)
	{
		MSG winMsg;
		while (PeekMessage(&winMsg, nullptr, 0, 0, PM_REMOVE))
		{
			//When program wants to exit, return false and assign the exit code.
			if (winMsg.message == WM_QUIT) {
				exitCodeOut = static_cast<int>(winMsg.wParam);
				return false;
			}

			TranslateMessage(&winMsg);
			DispatchMessage(&winMsg);
		}

		return true;
	}

	LRESULT HandleWindowProcedure(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
	{
		WindowEventCallbackFunc* CallbackFuncPtr = (WindowEventCallbackFunc*)GetWindowLongPtr(hWnd, GWLP_USERDATA);
		if (!CallbackFuncPtr)
		{
			return DefWindowProc(hWnd, msg, wParam, lParam);
			LAG_ASSERT("callbackFunc is nullptr in HandleWindowProcedure.");
		}
		WindowEventCallbackFunc Callback = *CallbackFuncPtr;
		switch (msg)
		{
		case WM_CLOSE:
		{
			PostQuitMessage(0);
			return 0;
		}
		break;
		case WM_SIZE:
		{
			//Get window sizes
			unsigned int newWinWidth = LOWORD(lParam);
			unsigned int newWinHeight = HIWORD(lParam);
			winData->windowWidth = newWinWidth, winData->windowHeight = newWinHeight;

			//Calculate non-client sizes
			RECT nonClientRect = { 0 };
			GetWindowRect(winData->hWnd, &nonClientRect);

			WindowResizeEvent newEvent = WindowResizeEvent(newWinWidth, newWinHeight, nonClientRect.right - nonClientRect.left, nonClientRect.bottom - nonClientRect.top);

			Renderer::OnResize();

			Callback(newEvent);
			

			std::cout << "Window resize: " << newWinWidth << ", " << newWinHeight << std::endl;
		}
		break;
		case WM_MOVE:
		{
			long newWindowXPos = LOWORD(lParam);
			long newWindowYPos = HIWORD(lParam);
			WindowMoveEvent newEvent = WindowMoveEvent(newWindowXPos, newWindowYPos);
			Callback(newEvent);

			std::cout << "Window move: " << newWindowXPos << ", " << newWindowYPos << std::endl;
		}
		break;
		}

		return DefWindowProc(hWnd, msg, wParam, lParam);
	}

	void SetWindowEventCallback(const WindowEventCallbackFunc& callbackFunc)
	{
		winData->winEventCallback = callbackFunc;
	}

	LAG_API void SetWindowName(const char* windowName)
	{
		winData->windowName = windowName;
		LAG::Utility::Logger::Error("SetWindowName NOT IMPLEMENTED YET!!!");
	}

	LAG_API unsigned int GetWidth()
	{
		return winData->windowWidth;
	}

	LAG_API unsigned int GetHeight()
	{
		return winData->windowHeight;
	}

	LAG_API unsigned int GetNonClientWidth()
	{
		RECT betterRect = { 0 };
		GetWindowRect(winData->hWnd, &betterRect);
		return betterRect.right;
	}

	LAG_API unsigned int GetNonClientHeight()
	{
		RECT betterRect = { 0 };
		GetWindowRect(winData->hWnd, &betterRect);
		return betterRect.bottom;
	}

	const void* GetWindowData()
	{
		return static_cast<void*>(winData);
	}
}