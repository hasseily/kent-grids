#include <SDL2/SDL.h> 
#include <SDL2/gfx/SDL2_gfxPrimitives.h>

#include <iostream>

#include "Imgui/imgui.h"
#include "Imgui/imgui_impl_sdl.h"
#include "Imgui/imgui_impl_sdlrenderer.h"

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

	// Setup Dear ImGui context
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO(); (void)io;

	// Setup Dear ImGui style
	ImGui::StyleColorsDark();

	// Setup Platform/Renderer backends
	ImGui_ImplSDL2_InitForSDLRenderer(window, renderer);
	ImGui_ImplSDLRenderer_Init(renderer);

	while (true) {
		ImGui_ImplSDL2_ProcessEvent(&event);
		if (SDL_PollEvent(&event)) {

			if (event.type == SDL_QUIT) {
				exit(0);
			}

		}

		// Start the Dear ImGui frame
		ImGui_ImplSDLRenderer_NewFrame();
		ImGui_ImplSDL2_NewFrame();
		ImGui::NewFrame();

		ImGui::Begin("Hello, world!");
		ImGui::Text("This is some useful text.");

		ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
		ImGui::End();

		ImGui::Render();
		SDL_SetRenderDrawColor(renderer, 124, 34, 151, 17);
		SDL_RenderClear(renderer);
		ImGui_ImplSDLRenderer_RenderDrawData(ImGui::GetDrawData());
		SDL_RenderPresent(renderer);

	}
	// Cleanup
	ImGui_ImplSDLRenderer_Shutdown();
	ImGui_ImplSDL2_Shutdown();
	ImGui::DestroyContext();

	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(window);
	SDL_Quit();
	return 0;
}