#include <Application/Window.h>


int app::Window::s_ScreenWidth = -1;
int app::Window::s_ScreenHeight = -1;
int app::Window::s_CanvasWidth = -1;
int app::Window::s_CanvasHeight = -1;


app::Window::Window(const std::string& windowTile, bool fullscreen, int width, int height)
    : m_windowTitle(windowTile), m_fullscreen(fullscreen), m_windowedWidth(width), m_windowedHeight(height)
{
    initialize();
}

app::Window::~Window()
{
    cleanup();
}

void app::Window::initialize()
{
    // TODO: INIT GAMECONTROLLER NEEDS TO BE A FLAG?
    // Initialize SDL
    if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_GAMECONTROLLER) < 0) {
        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "SDL could not initialize! SDL_Error: %s\n", SDL_GetError());
        SDL_assert(false);
    }

    // Set SDL attributes
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 4);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 1);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);

    SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
    SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);

    SDL_GL_SetAttribute(SDL_GL_MULTISAMPLEBUFFERS, 1);
    SDL_GL_SetAttribute(SDL_GL_MULTISAMPLESAMPLES, 16);


    // Init screen geometry
    SDL_Rect displayBounds;
    if (SDL_GetDisplayBounds(0, &displayBounds) != 0) {
        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Error getting display bounds: %s", SDL_GetError());
        SDL_assert(false);
    }
    s_ScreenWidth = displayBounds.w;
    s_ScreenHeight = displayBounds.h;
    s_CanvasWidth = m_fullscreen ? displayBounds.w : m_windowedWidth;
    s_CanvasHeight = m_fullscreen ? displayBounds.h : m_windowedHeight;

    // Create a window
    int windowWidth = m_fullscreen ? s_ScreenWidth : m_windowedWidth;
    int windowHeight = m_fullscreen ? s_ScreenHeight : m_windowedHeight;
    m_window = SDL_CreateWindow(m_windowTitle.c_str(),
        SDL_WINDOWPOS_UNDEFINED,
        SDL_WINDOWPOS_UNDEFINED,
        windowWidth,
        windowHeight,
        SDL_WINDOW_OPENGL);
    if (m_window == NULL) {
        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Window could not be created! SDL_Error: %s\n", SDL_GetError());
        SDL_assert(false);
    }

    if (m_fullscreen)
        SDL_SetWindowFullscreen(m_window, SDL_WINDOW_FULLSCREEN);
    else
        SDL_SetWindowFullscreen(m_window, 0);

    // Create an OpenGL context
    m_glContext = SDL_GL_CreateContext(m_window);
    if (m_glContext == NULL) {
        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "OpenGL context could not be created! SDL Error: %s\n", SDL_GetError());
        SDL_assert(false);
    }

    // Initialize OpenGL
    if (SDL_GL_SetSwapInterval(1) < 0) {
        SDL_LogWarn(SDL_LOG_CATEGORY_APPLICATION, "Warning: Unable to set VSync! SDL Error: %s\n", SDL_GetError());
    }

    // Load GL extensions using glad
    int version = gladLoadGL((GLADloadfunc)SDL_GL_GetProcAddress);
    if (version == 0) {
        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Failed to load GL extensions using glad.");
        SDL_assert(false);
    }

    // Successfully loaded OpenGL
    SDL_Log("Loaded OpenGL %d.%d\n", GLAD_VERSION_MAJOR(version), GLAD_VERSION_MINOR(version));

    // TODO: We should move this somewhere else
    glEnable(GL_DEPTH_TEST);

    // TODO: We should move this somewhere else
    // Open the first connected gamepad
    m_gameController = NULL;
    for (int i = 0; i < SDL_NumJoysticks(); ++i) {
        if (SDL_IsGameController(i)) {
            m_gameController = SDL_GameControllerOpen(i);

            SDL_Log("Opened Gamepad %d", i);
            SDL_Log("Name: %s", SDL_GameControllerName(m_gameController));
            SDL_Log("Mappings: %d", SDL_GameControllerNumMappings());
            break;
        }
    }
}

void app::Window::cleanup()
{
    // Destroy the window and the OpenGL context
    SDL_DestroyWindow(m_window);
    SDL_GL_DeleteContext(m_glContext);

    // Close the gamepad
    SDL_GameControllerClose(m_gameController);

    // Quit SDL
    SDL_Quit();
}

void app::Window::update()
{
    // Update a window with OpenGL rendering.
    SDL_GL_SwapWindow(m_window);
}
