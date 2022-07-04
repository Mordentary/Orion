#pragma once

#include"Orion/Input.h"
namespace Orion
{
	class WindowsInput : public Input
	{
	protected:
		virtual bool IsKeyPressedImpl(int keycode) override;

		virtual bool IsMouseButtonPressedImpl(int keycode) override;

		virtual  std::pair<float, float> GetMousePositionImpl() override;
		virtual int GetMouseXImpl() override;
		virtual int GetMouseYImpl() override;
	
	};
}


