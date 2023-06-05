#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <iostream>

#include "RenderWindow.hpp"
#include "PerlinNoise.hpp"

#define WIDTH 1280
#define HEIGHT 720 

int main(int argc, char* args[])
{
	if (SDL_Init(SDL_INIT_VIDEO) > 0)
		std::cout << "HEY.. SDL_Init HAS FAILED. SDL_ERROR: " << SDL_GetError() << std::endl;

	if (!(IMG_Init(IMG_INIT_PNG)))
		std::cout << "IMG_init has failed. Error: " << SDL_GetError() << std::endl;

	RenderWindow window("GAME v1.0", WIDTH, HEIGHT);
	PerlinNoise noise(80, 80, WIDTH, HEIGHT);
	noise.generatePerlin();

	bool gameRunning = true;

	SDL_Event event;

	while (gameRunning)
	{
		// Get our controls and events
		while (SDL_PollEvent(&event))
		{
			if (event.type == SDL_QUIT){
				gameRunning = false;
			}
		}

		window.clear();
		
		for(int i=0; i<noise.pixels.size(); i++){
			for(int j=0; j<noise.pixels[i].size(); j++){
				window.drawPixel(i, j, noise.pixels[i][j]);
			}
		}

		window.show();
		SDL_Delay(50);
	}

	window.cleanUp();
	SDL_Quit();

	return 0;
}