#include <Application/Time.h>


void app::Time::update()
{
    // Calculate elapsed time.
    Uint32 current_ticks = SDL_GetTicks();
    m_elapsedTicks = current_ticks - m_previousTicks;
    m_previousTicks = current_ticks;
}
