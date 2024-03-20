#include "WindowManager.h"
#include "Utility/Logger.h"

namespace LAG
{
    void WindowManager::Initialize()
    {
    }

    void WindowManager::Shutdown()
    {
        if (m_AdditionalWindows.size() > 0)
            for (int i = 0; i < m_AdditionalWindows.size(); i++)
            {
                if (m_AdditionalWindows[i] != nullptr)
                {
                    m_AdditionalWindows[i].reset();
                    m_AdditionalWindows.erase(m_AdditionalWindows.begin() + i);
                }
            }

        if(m_MainWindow != nullptr)
            m_MainWindow.reset();
        SetFocussedWindow(nullptr);
    }

    Window* WindowManager::AddWindow(unsigned int winWidth, unsigned int winHeight, const char* winName, bool fullscreen, bool useVSync, bool centerWindow)
    {
        if (winWidth <= 1 && winHeight <= 1)
        {
            // Print the window size here
            // Also keep in mind that there is a max resolution as well
            Logger::Error("Failed to add window: Invalid window size.");
            return nullptr;
        }

        std::unique_ptr<Window> newWindow = std::make_unique<Window>();

        // This will initialize GLFW again?
        // Don't think this is ever tested with multiple windows
        newWindow->Initialize(winWidth, winHeight, fullscreen, useVSync, centerWindow);
        newWindow->SetWindowName(winName);

        // I would look at QT on how they solved the API for this.
        // They have a central widget or a dedicated class for the Main Window
        // Although I'm not sure why you require more windows when you work on a game, the game is running in one window only right?
        // If this is going to be the base of creating an editor as well, you have IMGUI as dependency, use the functionality of that library to create your editor
        // Look at the docking branch of IMGUI
        if (m_MainWindow == nullptr)
        {
            m_MainWindow = std::move(newWindow);
            SetFocussedWindow(m_MainWindow.get());
            return m_MainWindow.get();
        }
        else
        {
            m_AdditionalWindows.emplace_back(std::move(newWindow));
            return m_AdditionalWindows.at(m_AdditionalWindows.size() - 1).get();
        }
    }

    bool WindowManager::RemoveWindow(std::unique_ptr<Window>& window)
    {
        if (window.get() == m_MainWindow.get())
        {
            window.reset();
            Shutdown();
            return true;
        }
        else
        {
            for (auto it = m_AdditionalWindows.begin(); it != m_AdditionalWindows.end();)
                if (window == *it)
                {
                    window.reset();
                    m_AdditionalWindows.erase(it);
                    return true;
                }
        }
        return false;
    }

    void WindowManager::Update()
    {
        //If the main window is closing, we should close all the additional windows.
        if (!m_MainWindow->HandleWindowMessages())
        {
            Shutdown();
            return;
        }

        m_MainWindow->Update();
        m_MainWindow->PresentFrame();

        for (size_t i = 0; i < m_AdditionalWindows.size(); i++)
        {
            Window* winPtr = m_AdditionalWindows[i].get();
            //First, handle window messages. If function returns false, the window should close.
            if (!winPtr->HandleWindowMessages())
            {
                m_AdditionalWindows[i].reset();
                m_AdditionalWindows.erase(m_AdditionalWindows.begin() + i);
                SetFocussedWindow(m_MainWindow.get());
                continue;
            }
            winPtr->Update();
            winPtr->PresentFrame();
        }
    }

    Window* WindowManager::GetPrimaryWindow() const
    {
        return m_MainWindow.get();
    }

    Window* WindowManager::GetFocussedWindow() const
    {
        return m_FocussedWindow;
    }

    bool WindowManager::AreWindowsOpen() const
    {
        return (m_MainWindow.get() != nullptr) || (m_AdditionalWindows.size() > 0);
    }

    void WindowManager::SetFocussedWindow(Window* window)
    {
        m_FocussedWindow = window;
    }
}