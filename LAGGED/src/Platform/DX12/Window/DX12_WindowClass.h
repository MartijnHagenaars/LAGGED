#pragma once
#include "Platform/Base/PlatformIncludes.h"

namespace LAG
{
	class WindowClass
	{
	public:
		static WindowClass& GetWinClass()
		{
			static WindowClass winClass;
			return winClass;
		}

		void Shutdown();

		bool SetupWindowClass(LRESULT(CALLBACK* WndProcPtr)(HWND, UINT, WPARAM, LPARAM));

		const HINSTANCE& GetHInstance() { return m_HInstance; }

		const char* GetWindowClassName() const { return m_WindowClassName; }

	private:
		WindowClass();
		WindowClass(const WindowClass&) = delete;

		~WindowClass();

		HINSTANCE m_HInstance = nullptr;
		WNDCLASSEX* m_WindowClass = nullptr;

		const char* m_WindowClassName = "";
	};
}


