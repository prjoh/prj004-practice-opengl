#pragma once


#include <Rendering/Camera.h>



namespace game
{

	class Game
	{
	public:
		Game();

		void update(float elapsedTimeS);
	private:
		core::Camera m_camera;
	};

}
