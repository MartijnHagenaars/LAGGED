#pragma once
#include <functional>
#include <unordered_set>

#include "Core/Defines.h"
#include "Core/Input/Input.h"

struct GLFWwindow;
namespace LAG
{
	class Window
	{
		friend class Engine;
	public:
		/// <summary>
		/// Handles the message pump
		/// &exitCodeOut: when program wants to exit, the exit code is assigned to the referenced argument.
		/// Return value: returns FALSE when the program wants to exit
		/// </summary>
		/// <returns></returns>
		bool HandleWindowMessages();

		void PresentFrame();

		void GetMousePosition(float& xPos, float& yPos);
		bool CheckButtonPress(const Input::InputActionData& inputType, bool onlyDetectSinglePress);

		void SetResizeCallBack(std::function<void(uint32_t, uint32_t)> resizeCallback) { m_ResizeCallbackFunc = resizeCallback; }

		void SetWindowTitle(const std::string& windowTitle);
		std::string GetWindowName() const { return m_WindowTitle; }

		bool IsOpen() const { return m_IsOpen; }
		void Close() { m_IsOpen = false; }

		unsigned int GetWidth() const { return m_WindowWidth; }
		unsigned int GetHeight() const { return m_WindowHeight; }

	private:
		Window(uint32_t winWidth, uint32_t winHeight, const std::string& winTitle, bool fullscreen, bool useVSync = true, bool centerWindow = true);

		Window(const Window& other) = delete;
		Window& operator=(Window& other) = delete;

		bool Initialize();
		void Shutdown();

		bool PlatformInitialize();
		bool ImGuiInitialize();

		void Update();

		//Window property values
		std::string m_WindowTitle;
		uint32_t m_WindowWidth, m_WindowHeight;
		bool m_IsFullscreen;
		bool m_UseVSync;

		//Callback functions
		std::function<void(uint32_t, uint32_t)> m_ResizeCallbackFunc;

		bool m_IsOpen = false;
		GLFWwindow* m_Window = nullptr;
		std::unordered_set<size_t> pressedButtonIDs;
	};
}