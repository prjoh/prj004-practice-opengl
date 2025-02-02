#include <Application/Application.h>


#include <Loading/ResourceManager.h>



std::string app::Application::s_Path;

app::Application::Application(const std::string& applicationName, bool fullscreenMode, int windowWidth, int windowHeight)
	: m_window(Window(applicationName, fullscreenMode, windowWidth, windowHeight))
{
    // Get base path
    char* basePath = SDL_GetBasePath();
    if (basePath == NULL) {
        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Couldn't get base path: %s", SDL_GetError());
        SDL_assert(false);
    }

    s_Path = basePath;

    SDL_free(basePath);
}

int app::Application::execute()
{
    core::ResourceManager& resourceManager = core::ResourceManager::getInstance();
    resourceManager.loadShaders();

    std::string basePath = app::Application::s_Path;
    std::string brickPath = basePath + "Assets\\Textures\\brick_diffuse.png";
    resourceManager.loadTextureFromFile(brickPath);

    // Main loop
    while (!app::Input::getInstance().quitEvent()) {

        m_input.update();
        
        m_time.update();

        //// Start the Dear ImGui frame
        //ImGui_ImplOpenGL3_NewFrame();
        //ImGui_ImplSDL2_NewFrame(window);
        //ImGui::NewFrame();

        //// Clear the screen  // TODO: Move this
        //glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
        //glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        //Execution::update(delta_time_s);
        m_game.update(m_time.elapsedS());

        //// Render imgui
        //ImGui::Render();
        //ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

        m_window.update();
    }

	return 0;
}
