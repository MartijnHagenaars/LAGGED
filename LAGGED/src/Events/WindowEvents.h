#pragma once
#include "EventBase.h"

namespace LAG
{
	class WindowResizeEvent : public EventBase
	{
	public:
		WindowResizeEvent(unsigned int newWinWidth, unsigned int newWinHeight, unsigned int winNonClientWidth, unsigned int winNonClientHeight) :
			EventBase(EventType::WindowResize), windowWidth(newWinWidth), windowHeight(newWinHeight), 
			windowNonClientWidth(winNonClientWidth), windowNonClientHeight(winNonClientHeight)
		{
			aspectRatio = static_cast<float>(windowHeight) / windowWidth;
		}

		unsigned int windowWidth = 0, windowHeight = 0;
		unsigned int windowNonClientWidth = 0, windowNonClientHeight = 0;
		float aspectRatio = 0.f;
	};

	class WindowMoveEvent : public EventBase
	{
	public:
		WindowMoveEvent(long newWindowXPos, long newWindowYPos) :
			EventBase(EventType::WindowMove), windowXPos(newWindowXPos), windowYPos(newWindowYPos)
		{ }

		long windowXPos = 0, windowYPos = 0;
	};

	class WindowFocus : public EventBase
	{
	public:
		WindowFocus() :
			EventBase(EventType::WindowFocus)
		{ }
	};

	class WindowUnfocus : public EventBase
	{
	public:
		WindowUnfocus() :
			EventBase(EventType::WindowUnfocus)
		{ }
	};


}