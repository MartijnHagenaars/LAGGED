#include "Precomp.h"
#include "DX12_WindowClass.h"
#include "Exceptions/DX12_WindowExceptionMacros.h"

namespace LAG
{
	WindowClass::WindowClass()
	{

	}

	WindowClass::~WindowClass()
	{
		Shutdown();
	}

	void WindowClass::Shutdown()
	{
		if (m_WindowClass != nullptr)
		{
			UnregisterClass(m_WindowClassName, m_HInstance);

			delete m_WindowClass;
			m_WindowClass = nullptr;
		}
	}

	bool WindowClass::SetupWindowClass(LRESULT(CALLBACK* WndProcPtr)(HWND, UINT, WPARAM, LPARAM))
	{
		if (m_WindowClass == nullptr)
			m_WindowClass = new WNDCLASSEX{ 0 };
		else return false;

		m_WindowClassName = "LAG_Window";
		m_HInstance = GetModuleHandle(nullptr);

		m_WindowClass->cbSize = sizeof(WNDCLASSEX);
		m_WindowClass->style = CS_OWNDC;
		m_WindowClass->lpfnWndProc = WndProcPtr;
		m_WindowClass->cbClsExtra = 0;
		m_WindowClass->cbWndExtra = 0;
		m_WindowClass->hInstance = GetHInstance();
		m_WindowClass->hIcon = nullptr;
		m_WindowClass->hCursor = nullptr;
		m_WindowClass->hbrBackground = nullptr;
		m_WindowClass->lpszMenuName = nullptr;
		m_WindowClass->lpszClassName = GetWindowClassName();

		RegisterClassEx(m_WindowClass);
		LAG_WINDOW_EXCEPTION_PREV();

		return (m_WindowClass != nullptr);
	}
}
