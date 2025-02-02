#include <Game/Game.h>

#include <glad/gl.h>


#include <Application/Input/Input.h>
#include <Application/Window.h>
#include <Geometry/Geometry.h>
#include <Geometry/Line.h>
#include <Rendering/Material.h>
#include <Rendering/Mesh.h>
#include <Rendering/SceneLights.h>

#include <iostream>


game::Game::Game()
    : m_camera({ 0.0f, 0.0f, 15.0f }, {0.0f, 0.0f, 0.0f}, core::g_WorldUp, (float)app::Window::s_CanvasWidth / (float)app::Window::s_CanvasHeight, 45.0f, 0.01f, 1000.0f)
{
}

void game::Game::update(float elapsedTimeS)
{
    //if (app::Input::getInstance().keyDown(SDLK_UP))
    //    std::cout << "SDLK_UP: DOWN" << std::endl;

    //if (app::Input::getInstance().keyPressed(SDLK_UP))
    //    std::cout << "SDLK_UP: PRESSED" << std::endl;

    //if (app::Input::getInstance().keyUp(SDLK_UP))
    //    std::cout << "SDLK_UP: UP" << std::endl;

    //if (app::Input::getInstance().mouseButtonDown(SDL_BUTTON_LEFT))
    //    std::cout << "SDL_BUTTON_LEFT: DOWN" << std::endl;

    //if (app::Input::getInstance().mouseButtonPressed(SDL_BUTTON_LEFT))
    //{
    //    std::cout << "SDL_BUTTON_LEFT: PRESSED" << std::endl;
    //    std::cout << "Mouse(" << app::Input::getInstance().mouseX() << "," << app::Input::getInstance().mouseY() << ") | MouseDelta(" << app::Input::getInstance().mouseDeltaX() << "," << app::Input::getInstance().mouseDeltaY() << ") | WheelY = " << app::Input::getInstance().mouseWheelY() << std::endl;
    //}

    //if (app::Input::getInstance().mouseButtonUp(SDL_BUTTON_LEFT))
    //    std::cout << "SDL_BUTTON_LEFT: UP" << std::endl;

    //if (app::Input::getInstance().controllerButtonDown(SDL_CONTROLLER_BUTTON_RIGHTSHOULDER))
    //    std::cout << "SDL_CONTROLLER_BUTTON_RIGHTSHOULDER: DOWN" << std::endl;

    //if (app::Input::getInstance().controllerButtonPressed(SDL_CONTROLLER_BUTTON_RIGHTSHOULDER))
    //    std::cout << "SDL_CONTROLLER_BUTTON_RIGHTSHOULDER: PRESSED" << std::endl;

    //if (app::Input::getInstance().controllerButtonUp(SDL_CONTROLLER_BUTTON_RIGHTSHOULDER))
    //    std::cout << "SDL_CONTROLLER_BUTTON_RIGHTSHOULDER: UP" << std::endl;

    //if (app::Input::getInstance().controllerButtonPressed(SDL_CONTROLLER_BUTTON_RIGHTSHOULDER) && app::Input::getInstance().controllerAxis(SDL_CONTROLLER_AXIS_LEFTX) != 0)
    //    std::cout << "SDL_CONTROLLER_AXIS_LEFTX: " << app::Input::getInstance().controllerAxis(SDL_CONTROLLER_AXIS_LEFTX) << std::endl;

    if (app::Input::getInstance().mouseButtonPressed(SDL_BUTTON_LEFT))
        m_camera.move(elapsedTimeS);

    // TODO: BlinnPhong just looks weird  <<  FIX THIS
    // TODO: More Materials
    // TODO: Terrain System
    // TODO: Box Collider, Sphere Collider and Capsule Collider / Mesh Collider
    // TODO: https://gameprogrammingpatterns.com/command.html
    // TODO: A Camera is a Component which requires a Transform Component to be present on the current Entity!

    core::Line xAxis({ 0.0f, 0.0f, 0.0f }, { 10.0f, 0.0f, 0.0f }, { 255.0f, 0.0f, 0.0f });
    core::Line yAxis({ 0.0f, 0.0f, 0.0f }, { 0.0f, 10.0f, 0.0f }, { 0.0f, 255.0f, 0.0f });
    core::Line zAxis({ 0.0f, 0.0f, 0.0f }, { 0.0f, 0.0f, 10.0f }, { 0.0f, 0.0f, 255.0f });

    core::DirectionalLight directionalLight;
    directionalLight.position = { 50.0f, 50.0f, 0.0f };
    
    core::SceneLights sceneLights;
    sceneLights.ambientLight.color = { 0.0f, 0.0f, 0.0f };
    sceneLights.directionalLights.push_back(directionalLight);

    // TODO: Camera does not respect aspect ratio of screen!!!
    core::Triangle triangle(
        { -0.5f, -0.5f, 0.0f },
        { 0.5f, -0.5f, 0.0f },
        { 0.0f,  0.5f, 0.0f }
    );
    core::Plane plane(1, 2);
    core::Cube cube(2, 2, 2);
    core::Sphere sphere(4, 32, 16, 0.0f, 6.283185307179586f, 0, 3.141592653589793f);
    //core::BasicMaterial material({ 1.0f, 0.5f, 0.2f });
    core::BasicMaterial basicMaterial({ 1.0f, 1.0f, 1.0f }, "brick_diffuse.png");
    core::BlinnPhongMaterial phongMaterial(glm::vec3(1.0f), glm::vec3(1.0f), glm::vec3(1.0f), 30.0f);

    core::Mesh mesh;
    //mesh.m_geometry = &cube;
    //mesh.m_material = &basicMaterial;
    mesh.m_geometry = &sphere;  // TODO: So pointers should really not point into the stack!
    mesh.m_material = &phongMaterial;  // TODO: So pointers should really not point into the stack!

    //mesh.m_transform.position({ 5.0f, 5.0f, 0.0f });

    //glm::vec3 pos = { 0.05f, 0.0f, 0.0f };
    //camera.m_transform.localPosition(pos);

    // TODO: Move this
    // Clear the screen
    glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    xAxis.draw(m_camera.viewMatrix(), m_camera.projectionMatrix(), m_camera.position(), sceneLights);
    yAxis.draw(m_camera.viewMatrix(), m_camera.projectionMatrix(), m_camera.position(), sceneLights);
    zAxis.draw(m_camera.viewMatrix(), m_camera.projectionMatrix(), m_camera.position(), sceneLights);

    mesh.draw(m_camera.viewMatrix(), m_camera.projectionMatrix(), m_camera.position(), sceneLights);
}
