#pragma once
#include "WindowBase.h"

//Not super happy with this. Need to fine a better solution
#ifdef PLATFORM_OPENGL
#include "Platform/OpenGL/Window/GL_Window.h"
#elif PLATFORM_DIRECTX
#include "Platform/DX12/Window/DX12_Window.h"
#endif


namespace LAG
{
	class WindowManager
	{
		friend class Window;
	public:

		WindowManager(const WindowManager&) = delete;
		static WindowManager& Get()
		{
			static WindowManager instance;
			return instance;
		}
		void Shutdown();

		Window* AddWindow(unsigned int winWidth, unsigned int winHeight, const char* winName, bool fullscreen, bool useVSync = true, bool centerWindow = true);
		bool RemoveWindow(std::unique_ptr<Window>& window);
		
		void Update();

		//Returns a shared pointer to the primary window. 
		Window* GetPrimaryWindow() const;

		//Returns a shared pointer to the window that the user currently has selected
		Window* GetFocussedWindow() const;

		bool AreWindowsOpen() const;
	private:
		WindowManager() {};
		~WindowManager() {};

		void SetFocussedWindow(Window* window);

		std::unique_ptr<Window> m_MainWindow;
		std::vector<std::unique_ptr<Window>> m_AdditionalWindows;

		Window* m_FocussedWindow;
	};
}


