#include <SDL2/SDL.h> 
#include <SDL2/gfx/SDL2_gfxPrimitives.h>

#include <iostream>


int main(int argc, char* argv[]) {
	SDL_Init(SDL_INIT_EVERYTHING);
	SDL_Renderer* renderer;
	SDL_Window* window;
	SDL_Event event;

	int resX = 600, resY = 400;

	window = SDL_CreateWindow("SDL2 Test Window", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, resX, resY, SDL_WINDOW_RESIZABLE);
	renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);


	SDL_SetRenderDrawColor(renderer, 0, 0, 0, 0);
	SDL_RenderClear(renderer);
	SDL_RenderPresent(renderer);

	while (true) {
		if (SDL_PollEvent(&event)) {

			if (event.type == SDL_QUIT) {
				exit(0);
			}

			if (event.type == SDL_MOUSEBUTTONDOWN) {

				if (event.button.button == SDL_BUTTON_LEFT) {

					SDL_SetRenderDrawColor(renderer, 124, 34, 151, 17);
					SDL_RenderClear(renderer);
					SDL_RenderPresent(renderer);

				}

			}

		}

	}

	SDL_DestroyWindow(window);
	SDL_Quit();
	return 0;
}