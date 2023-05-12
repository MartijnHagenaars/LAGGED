#pragma once

namespace LAG
{
	enum class EventTypeCategory
	{
		Undefined = 0,
		WindowAdjustment,
		KeyboardInput,
		MouseInput
	};

	enum class EventType
	{
		Undefined = 0,
		WindowResize,
		WindowMove,
		WindowFocus,
		WindowUnfocus,

		KeyboardPress,
		KeyboardRelease,

		MouseButtonPress,
		MouseButtonRelease,
		MouseMove,
		MouseScroll
	};

	class EventBase
	{
	public: 
		EventBase(EventType eventType) :
			m_EventType(eventType)
		{
			if (m_EventType != EventType::Undefined)
			{
				if (m_EventType >= EventType::WindowResize && m_EventType <= EventType::WindowUnfocus)
					m_EventTypeCategory = EventTypeCategory::WindowAdjustment;
				else if (m_EventType >= EventType::KeyboardPress && m_EventType <= EventType::KeyboardRelease)
					m_EventTypeCategory = EventTypeCategory::KeyboardInput;
				else if (m_EventType >= EventType::MouseButtonPress && m_EventType <= EventType::MouseScroll)
					m_EventTypeCategory = EventTypeCategory::MouseInput;
				else m_EventTypeCategory = EventTypeCategory::Undefined;
			}
		
		}
		virtual ~EventBase() {};

		EventType GetEventType() const { return m_EventType; }
		EventTypeCategory GetEventCategory() const { return m_EventTypeCategory; }

	protected:
		EventType m_EventType = EventType::Undefined;

	private:
		EventTypeCategory m_EventTypeCategory = EventTypeCategory::Undefined;
	};
}