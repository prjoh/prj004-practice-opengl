#pragma once

#include <Sdl.h>


namespace app {

	class Time
	{
	public:
		Time() = default;

		void update();
		Uint32 elapsedMS() { return m_elapsedTicks; }
		float elapsedS() { return m_elapsedTicks / 1000.0f; }
	private:
		Uint32 m_previousTicks = 0;
		Uint32 m_elapsedTicks = 0;
	};

}
