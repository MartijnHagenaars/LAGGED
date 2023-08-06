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
                    m_AdditionalWindows.erase(m_AdditionalWindows.begin() + i);
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

    bool WindowManager::RemoveWindow(std::shared_ptr<Window>& window)
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
            std::shared_ptr<Window> winPtr = m_AdditionalWindows[i];
            //First, handle window messages. If function returns false, the window should close.
            if (!winPtr->HandleWindowMessages())
            {
                m_AdditionalWindows[i].reset();
                m_AdditionalWindows.erase(m_AdditionalWindows.begin() + i);
                continue;
            }
            winPtr->Update();
            winPtr->PresentFrame();
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

    bool WindowManager::AreWindowsOpen() const
    {
        return (m_MainWindow.get() != nullptr) || (m_AdditionalWindows.size() > 0);
    }

    void WindowManager::SetFocussedWindow(std::shared_ptr<Window> window)
    {
        m_FocussedWindow = window;
    }
}