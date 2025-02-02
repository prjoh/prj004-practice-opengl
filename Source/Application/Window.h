#pragma once

#include <glad/gl.h>
#include <SDL.h>
#include <string>


namespace app {
	
	// TODO: We may want to make this abstract and then have an OpenGL window implementation
	class Window
	{

	public:
		static int s_ScreenWidth;
		static int s_ScreenHeight;
		static int s_CanvasWidth;
		static int s_CanvasHeight;

		Window(const std::string& windowTile, bool fullscreen, int width = 800, int height = 600);
		virtual ~Window();

		void update();

	private:
		SDL_Window* m_window;
		SDL_GLContext m_glContext;

		SDL_GameController* m_gameController;

		std::string m_windowTitle;
		bool m_fullscreen;

		int m_windowedWidth;
		int m_windowedHeight;

		void initialize();
		void cleanup();
	};

}
