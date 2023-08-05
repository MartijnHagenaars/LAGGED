#include "Precomp.h"
#include "WindowManager.h"

namespace LAG
{
    void WindowManager::Shutdown()
    {
        if (m_AdditionalWindows.size() > 0)
            for (int i = 0; i < m_AdditionalWindows.size(); i++)
            {
                if (m_AdditionalWindows[i] != nullptr)
                {
                    m_AdditionalWindows[i].reset();
                }
            }

        if(m_MainWindow != nullptr)
            m_MainWindow.reset();
    }

    std::shared_ptr<Window> WindowManager::AddWindow(unsigned int winWidth, unsigned int winHeight, const char* winName, bool fullscreen, bool useVSync, bool centerWindow)
    {
        if (winWidth <= 1 && winHeight <= 1)
        {
            Utility::Logger::Error("Failed to add window: Invalid window size.");
            return nullptr;
        }

        std::shared_ptr<Window> newWindow = std::make_shared<Window>();
        newWindow->Initialize(winWidth, winHeight, fullscreen, useVSync, centerWindow);
        newWindow->SetWindowName(winName);

        if (m_MainWindow == nullptr)
        {
            m_MainWindow = newWindow;
            SetFocussedWindow(newWindow);
        }
        else m_AdditionalWindows.emplace_back(newWindow);

        return newWindow;
    }

    bool WindowManager::RemoveWindow(const std::shared_ptr<Window>& window)
    {
        return false;
    }

    void WindowManager::Update()
    {
        m_MainWindow->Update();
        m_MainWindow->PresentFrame();

        //Move this!
        glfwPollEvents();

        //Update other windows (if any exist)
        if (m_AdditionalWindows.size() > 0)
            for (auto i = m_AdditionalWindows.begin(); i != m_AdditionalWindows.end();)
            {
                if (!(*i)->HandleWindowMessages())
                {
                    RemoveWindow((*i++));
                    continue;
                }
                (*i)->Update();
                (*i)->PresentFrame();
                ++i;
            }
    }

    std::shared_ptr<Window> WindowManager::GetPrimaryWindow() const
    {
        return m_MainWindow;
    }

    std::shared_ptr<Window> WindowManager::GetFocussedWindow() const
    {
        return m_FocussedWindow;
    }

    void WindowManager::SetFocussedWindow(std::shared_ptr<Window> window)
    {
        m_FocussedWindow = window;
    }
}