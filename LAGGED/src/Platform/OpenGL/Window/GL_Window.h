#pragma once
#include "Platform/Base/Window/WindowBase.h"

#include "GL/glew.h"
#include "GLFW/glfw3.h"

#include <unordered_set>

namespace LAG
{
	class EventBase;
	using WindowEventCallbackFunc = std::function<void(EventBase&)>;

	class Window : public WindowBase
	{
		friend class WindowManager;
	public:
		Window();
		~Window() override;

		bool HandleWindowMessages() override;

		void PresentFrame() override;

		bool CheckButtonPress(const Input::InputActionData& inputType, bool onlyDetectSinglePress) override;
		void GetMousePosition(float& xPos, float& yPos) override;

		void SetWindowEventCallback(const WindowEventCallbackFunc& callbackFunc) override;

		LAG_API void SetWindowName(const char* windowName) override;
		
		LAG_API unsigned int GetNonClientWidth() override;
		LAG_API unsigned int GetNonClientHeight() override;

	private:
		void Initialize(unsigned int winWidth, unsigned int winHeight, bool fullscreen, bool useVSync = true, bool centerWindow = true) override;
		void SetupCallbackFunctions();

		void Update() override;

		void WindowResizeCallback(GLFWwindow* window, int width, int height);

		GLFWwindow* m_Window = nullptr;
		WindowEventCallbackFunc m_WinEventCallback = NULL;

		std::unordered_set<size_t> pressedButtonIDs;

	};
}