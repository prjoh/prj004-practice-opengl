#pragma once

#include <Application/Input/InputData.h>

#include <SDL.h>


namespace app {

	class InputHandler
	{
	public:
		InputHandler() = default;

		void update();
	private:
		SDL_Event m_event;
	};

}
