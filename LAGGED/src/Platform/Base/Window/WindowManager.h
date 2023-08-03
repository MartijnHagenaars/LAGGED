#pragma once
#include "WindowBase.h"

namespace LAG
{
	class WindowManager
	{
	public:

		WindowManager(const WindowManager&) = delete;
		static WindowManager& Get()
		{
			static WindowManager instance;
			return instance;
		}
		void Shutdown();

		std::shared_ptr<Window> AddWindow(unsigned int winWidth, unsigned int winHeight, bool fullscreen, bool useVSync = true, bool centerWindow = true);
		bool RemoveWindow(const std::shared_ptr<Window>& window);
		
		void Update();

		std::shared_ptr<Window> GetPrimaryWindow() const;

	private:
		WindowManager() {};
		~WindowManager() {};

		std::shared_ptr<Window> m_MainWindow;
		std::vector<std::shared_ptr<Window>> m_AdditionalWindows;
	};
}


