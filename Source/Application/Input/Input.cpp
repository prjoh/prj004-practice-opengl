#include <Application/Input/Input.h>
#include <Application/Input/InputData.h>


#include <iostream>


void app::InputHandler::update()
{
    app::Input& inputData = app::Input::getInstance();

    for (auto& [_, v] : inputData.m_isKeyDown) v = false;
    for (auto& [_, v] : inputData.m_isKeyUp) v = false;
    for (auto& [_, v] : inputData.m_isMouseButtonDown) v = false;
    for (auto& [_, v] : inputData.m_isMouseButtonUp) v = false;
    for (auto& [_, v] : inputData.m_isControllerButtonDown) v = false;
    for (auto& [_, v] : inputData.m_isControllerButtonUp) v = false;
    inputData.m_mouseDeltaX = 0;
    inputData.m_mouseDeltaY = 0;
    inputData.m_mouseWheelX = 0;
    inputData.m_mouseWheelY = 0;

    while (SDL_PollEvent(&m_event)) {

        switch (m_event.type) {
            case SDL_QUIT:
            {
                inputData.m_quit = true;
                break;
            }

            case SDL_KEYDOWN:
            {
                bool lastFramePressed = inputData.keyPressed(m_event.key.keysym.sym);
                inputData.m_isKeyDown[m_event.key.keysym.sym] = true && !lastFramePressed;
                inputData.m_isKeyPressed[m_event.key.keysym.sym] = true;
                break;
            }

            case SDL_KEYUP:
            {
                inputData.m_isKeyUp[m_event.key.keysym.sym] = true;
                inputData.m_isKeyPressed[m_event.key.keysym.sym] = false;
                break;
            }

            case SDL_MOUSEBUTTONDOWN:
            {
                bool lastFramePressed = inputData.mouseButtonPressed(m_event.button.button);
                inputData.m_isMouseButtonDown[m_event.button.button] = true && !lastFramePressed;
                inputData.m_isMouseButtonPressed[m_event.button.button] = true;
                break;
            }

            case SDL_MOUSEBUTTONUP:
            {
                inputData.m_isMouseButtonUp[m_event.button.button] = true;
                inputData.m_isMouseButtonPressed[m_event.button.button] = false;
                break;
            }

            case SDL_MOUSEMOTION:
            {
                inputData.m_mouseX = m_event.motion.x;
                inputData.m_mouseY = m_event.motion.y;
                inputData.m_mouseDeltaX = m_event.motion.xrel;
                inputData.m_mouseDeltaY = m_event.motion.yrel;
                break;
            }

            case SDL_MOUSEWHEEL:
            {
                std::cout << m_event.wheel.y << std::endl;
                inputData.m_mouseWheelX = m_event.wheel.x;
                inputData.m_mouseWheelY = m_event.wheel.y;
                break;
            }

            case SDL_CONTROLLERBUTTONDOWN:
            {
                bool lastFramePressed = inputData.controllerButtonPressed(m_event.cbutton.button);
                inputData.m_isControllerButtonDown[m_event.cbutton.button] = true && !lastFramePressed;
                inputData.m_isControllerButtonPressed[m_event.cbutton.button] = true;
                break;
            }

            case SDL_CONTROLLERBUTTONUP:
            {
                inputData.m_isControllerButtonUp[m_event.cbutton.button] = true;
                inputData.m_isControllerButtonPressed[m_event.cbutton.button] = false;
                break;
            }

            case SDL_CONTROLLERAXISMOTION:
            {
                inputData.m_controllerAxisValues[m_event.caxis.axis] = (float)m_event.caxis.value / (float)SDL_JOYSTICK_AXIS_MIN;
                break;
            }

        }

        //// Pass event to Dear ImGui
        //ImGui_ImplSDL2_ProcessEvent(&input.event);
    }
}
