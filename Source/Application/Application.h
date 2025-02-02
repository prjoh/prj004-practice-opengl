#pragma once


#include <Application/Window.h>
#include <Application/Input/Input.h>
#include <Application/Time.h>
#include <Game/Game.h>


namespace app {

	class Application
	{

	public:
		Application(const std::string& applicationName, bool fullscreenMode = true, int windowWidth = 800, int windowHeight = 600);

		static std::string s_Path;

		int execute();
	private:
		app::Window m_window;
		app::InputHandler m_input;
		app::Time m_time;
		game::Game m_game;
	};

}
