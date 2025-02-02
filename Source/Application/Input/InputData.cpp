#include <Application/Input/InputData.h>


bool app::Input::controllerButtonPressed(Uint8 button) const
{
	auto pos = m_isControllerButtonPressed.find(button);
	if (pos == m_isControllerButtonPressed.end()) {
		return false;
	}
	return pos->second;
}

bool app::Input::controllerButtonDown(Uint8 button) const
{
	auto pos = m_isControllerButtonDown.find(button);
	if (pos == m_isControllerButtonDown.end()) {
		return false;
	}
	return pos->second;
}

bool app::Input::controllerButtonUp(Uint8 button) const
{
	auto pos = m_isControllerButtonUp.find(button);
	if (pos == m_isControllerButtonUp.end()) {
		return false;
	}
	return pos->second;
}

float app::Input::controllerAxis(Uint8 axis) const
{
	auto pos = m_controllerAxisValues.find(axis);
	if (pos == m_controllerAxisValues.end()) {
		return 0.0f;
	}
	return pos->second;
}

bool app::Input::mouseButtonPressed(Uint8 mouseButton) const
{
	auto pos = m_isMouseButtonPressed.find(mouseButton);
	if (pos == m_isMouseButtonPressed.end()) {
		return false;
	}
	return pos->second;
}

bool app::Input::mouseButtonDown(Uint8 mouseButton) const
{
	auto pos = m_isMouseButtonDown.find(mouseButton);
	if (pos == m_isMouseButtonDown.end()) {
		return false;
	}
	return pos->second;
}

bool app::Input::mouseButtonUp(Uint8 mouseButton) const
{
	auto pos = m_isMouseButtonUp.find(mouseButton);
	if (pos == m_isMouseButtonUp.end()) {
		return false;
	}
	return pos->second;
}

bool app::Input::keyPressed(SDL_Keycode keyCode) const
{
	auto pos = m_isKeyPressed.find(keyCode);
	if (pos == m_isKeyPressed.end()) {
		return false;
	}
	return pos->second;
}

bool app::Input::keyDown(SDL_Keycode keyCode) const
{
	auto pos = m_isKeyDown.find(keyCode);
	if (pos == m_isKeyDown.end()) {
		return false;
	}
	return pos->second;
}

bool app::Input::keyUp(SDL_Keycode keyCode) const
{
	auto pos = m_isKeyUp.find(keyCode);
	if (pos == m_isKeyUp.end()) {
		return false;
	}
	return pos->second;
}
