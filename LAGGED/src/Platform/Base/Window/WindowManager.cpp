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

    std::shared_ptr<Window> WindowManager::AddWindow(unsigned int winWidth, unsigned int winHeight, bool fullscreen, bool useVSync, bool centerWindow)
    {
        if (winWidth <= 1 && winHeight <= 1)
        {
            Utility::Logger::Error("Failed to add window: Invalid window size.");
            return nullptr;
        }

        std::shared_ptr<Window> newWindow = std::make_shared<Window>();
        newWindow->Initialize(winWidth, winHeight, fullscreen, useVSync, centerWindow);
        if (newWindow == nullptr)
        {
            printf("FUCK\n");
        }

        if (m_MainWindow == nullptr)
            m_MainWindow = newWindow;
        else m_AdditionalWindows.emplace_back(newWindow);
        
        if (m_MainWindow == nullptr)
        {
            printf("FUCK\n");
        }

        return newWindow;
    }

    bool WindowManager::RemoveWindow(const std::shared_ptr<Window>& window)
    {
        return false;
    }

    void WindowManager::Update()
    {
        m_MainWindow->Update();

        //Update other windows (if any exist)
        if (m_AdditionalWindows.size() > 0)
            for (const auto& i : m_AdditionalWindows)
                i->Update();
    }

    std::shared_ptr<Window> WindowManager::GetPrimaryWindow() const
    {
        return m_MainWindow;
    }
}