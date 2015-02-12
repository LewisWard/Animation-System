// Author  : Lewis Ward
// Program : Animation System Tech Demo
// Date    : 09/02/2015
#include "DemoApplication.h"

int main(int argc, char *argv[])
{
	initSDL();
	Application application;
	initGLEW();

	bool gameloop = true;


	while (gameloop)
	{
		application.update();
		application.draw();
		gameloop = false;
	}

	application.~Application();

	// quit SDL
	SDL_Quit();
	return 0;
}
