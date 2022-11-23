#pragma once

#include"Event.h"



namespace Orion
{
	class ORION_API KeyEvent : public Event
	{
	public:
		inline int GetKeyCode() const { return m_KeyCode; }

		EVENT_CLASS_CATEGORY(EventCategoryKeyboard | EventCategoryInput)
	protected:
		KeyEvent(int keyCode) : m_KeyCode(keyCode) {}
		int m_KeyCode;
	};


	class ORION_API KeyPressedEvent : public KeyEvent
	{
	public: 
		KeyPressedEvent(int keyCode, int repeatCount, int mode) 
			: KeyEvent(keyCode), m_RepeatCount(repeatCount), m_Mode(mode) 
		{
		}

		inline int GetRepeatedCount() const { return m_RepeatCount; }
		inline int GetMode() const { return m_Mode; }
		std::string ToString()const override 
		{
			std::stringstream ss;
			ss << "KeyPressedEvent: " << " (" << m_RepeatCount << "repeats)";
			return ss.str();

		}
		EVENT_CLASS_TYPE(KeyPressed)
	private:
		int m_Mode;
		int m_RepeatCount;
	};


	class ORION_API KeyTypedEvent : public KeyEvent
	{
	public:
		KeyTypedEvent(int keyCode)
			: KeyEvent(keyCode)
		{
		}

		
		std::string ToString()const override
		{
			std::stringstream ss;
			ss << "KeyTypedEvent:" << m_KeyCode;
			return ss.str();

		}
		EVENT_CLASS_TYPE(KeyTyped)
	};


	class ORION_API KeyReleasedEvent : public KeyEvent
	{
	public:
		KeyReleasedEvent(int keyCode) : KeyEvent(keyCode) {}
		std::string ToString()const override
		{
			std::stringstream ss;
			ss << "KeyReleasedEvent: " << m_KeyCode;
			return ss.str();

		}
		EVENT_CLASS_TYPE(KeyReleased)

	};
}
