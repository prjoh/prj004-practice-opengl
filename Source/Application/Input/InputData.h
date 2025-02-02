#pragma once


#include <Util/Singleton.h>
#include <SDL.h>

#include <map>


namespace app
{
	class Input : public util::Singleton<Input> {
        friend class Singleton<Input>;
        friend class InputHandler;
    public:
        bool quitEvent() const { return m_quit; };

        bool controllerButtonPressed(Uint8 button) const;
        bool controllerButtonDown(Uint8 button) const;
        bool controllerButtonUp(Uint8 button) const;
        float controllerAxis(Uint8 axis) const;

        bool mouseButtonPressed(Uint8 mouseButton) const;
        bool mouseButtonDown(Uint8 mouseButton) const;
        bool mouseButtonUp(Uint8 mouseButton) const;
        Sint32 mouseX() const { return m_mouseX; };
        Sint32 mouseY() const { return m_mouseY; };
        Sint32 mouseDeltaX() const { return m_mouseDeltaX; };
        Sint32 mouseDeltaY() const { return m_mouseDeltaY; };
        Sint32 mouseWheelX() const { return m_mouseWheelX; };
        Sint32 mouseWheelY() const { return m_mouseWheelY; };

        bool keyPressed(SDL_Keycode keyCode) const;
        bool keyDown(SDL_Keycode keyCode) const;
        bool keyUp(SDL_Keycode keyCode) const;
    private:
        Input() = default;
        ~Input() = default;

        bool m_quit = false;

        /*
        SDLK_{keyname}
        */
        std::map<SDL_Keycode, bool> m_isKeyDown;
        std::map<SDL_Keycode, bool> m_isKeyUp;
        std::map<SDL_Keycode, bool> m_isKeyPressed;

        /*
        SDL_BUTTON_LEFT
        SDL_BUTTON_MIDDLE
        SDL_BUTTON_RIGHT
        SDL_BUTTON_X1
        SDL_BUTTON_X2
        */
        std::map<Uint8, bool> m_isMouseButtonDown;
        std::map<Uint8, bool> m_isMouseButtonUp;
        std::map<Uint8, bool> m_isMouseButtonPressed;

        Sint32 m_mouseX;
        Sint32 m_mouseY;
        Sint32 m_mouseDeltaX;
        Sint32 m_mouseDeltaY;
        Sint32 m_mouseWheelX;
        Sint32 m_mouseWheelY;

        /*
        SDL_CONTROLLER_BUTTON_INVALID
        SDL_CONTROLLER_BUTTON_A
        SDL_CONTROLLER_BUTTON_B
        SDL_CONTROLLER_BUTTON_X
        SDL_CONTROLLER_BUTTON_Y
        SDL_CONTROLLER_BUTTON_BACK
        SDL_CONTROLLER_BUTTON_GUIDE
        SDL_CONTROLLER_BUTTON_START
        SDL_CONTROLLER_BUTTON_LEFTSTICK
        SDL_CONTROLLER_BUTTON_RIGHTSTICK
        SDL_CONTROLLER_BUTTON_LEFTSHOULDER
        SDL_CONTROLLER_BUTTON_RIGHTSHOULDER
        SDL_CONTROLLER_BUTTON_DPAD_UP
        SDL_CONTROLLER_BUTTON_DPAD_DOWN
        SDL_CONTROLLER_BUTTON_DPAD_LEFT
        SDL_CONTROLLER_BUTTON_DPAD_RIGHT
        SDL_CONTROLLER_BUTTON_MISC1
        SDL_CONTROLLER_BUTTON_PADDLE1
        SDL_CONTROLLER_BUTTON_PADDLE2
        SDL_CONTROLLER_BUTTON_PADDLE3
        SDL_CONTROLLER_BUTTON_PADDLE4
        SDL_CONTROLLER_BUTTON_TOUCHPAD
        SDL_CONTROLLER_BUTTON_MAX
        */
        std::map<Uint8, bool> m_isControllerButtonDown;
        std::map<Uint8, bool> m_isControllerButtonUp;
        std::map<Uint8, bool> m_isControllerButtonPressed;

        /*
        SDL_CONTROLLER_AXIS_INVALID
        SDL_CONTROLLER_AXIS_LEFTX
        SDL_CONTROLLER_AXIS_LEFTY
        SDL_CONTROLLER_AXIS_RIGHTX
        SDL_CONTROLLER_AXIS_RIGHTY
        SDL_CONTROLLER_AXIS_TRIGGERLEFT
        SDL_CONTROLLER_AXIS_TRIGGERRIGHT
        SDL_CONTROLLER_AXIS_MAX
        */
        std::map<Uint8, float> m_controllerAxisValues;
	};
}
