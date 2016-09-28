// Author : Lewis Ward (http://www.lewis-ward.com)
// Program : Animation System Tech Demo
// Date    : 09/02/2015
// © 2015-2016 Lewis Ward. All rights reserved.
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

	// quit SDL
	SDL_Quit();
	return 0;
}
