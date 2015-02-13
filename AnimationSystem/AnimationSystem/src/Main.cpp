// Author  : Lewis Ward
// Program : Animation System Tech Demo
// Date    : 09/02/2015
#include "DemoApplication.h"

int main(int argc, char *argv[])
{
	// setup SDL, delta time and load the application
	bool gameloop = initSDL();
	Application application;
	initTimerDT();

	// setup GLEW
	initGLEW();

	// delta time
	float deltaTime;

	// gameloop
	while (gameloop)
	{
		// update the delta time
		deltaTime = updateTimerDT();

		// update and draw the application
		application.update(deltaTime);
		application.draw();
		gameloop = application.getGameloop();
	}

	application.~Application();

	// quit SDL
	SDL_Quit();
	return 0;
}
