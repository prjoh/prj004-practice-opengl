#include <iostream>
#include <iomanip>

#define GLAD_GL_IMPLEMENTATION
#define SDL_MAIN_HANDLED
#include <Application/Application.h>

//#define STB_IMAGE_IMPLEMENTATION    
//#include "stb_image.h"


int main(int argc, char* argv[])
{
    app::Application application("MyGame", false);
    return application.execute();
}
