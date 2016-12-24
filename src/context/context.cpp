#include "context.hpp"

#include <iostream>
#include <SDL.h>

static SDL_Window *window = nullptr;
static SDL_Renderer *renderer = nullptr;
static SDL_Texture *texture = nullptr;

static bool shouldQuit = false;

bool Context::SetupContext () {
	if (SDL_Init(SDL_INIT_VIDEO) != 0){
		std::cout << "SDL_Init Error: " << SDL_GetError() << std::endl;
		return false;
	}

	int scale = 10;
	int flags = SDL_WINDOW_RESIZABLE;
	window = SDL_CreateWindow("Chip8 Emulator",
	                          SDL_WINDOWPOS_CENTERED,
	                          SDL_WINDOWPOS_CENTERED,
	                          64 * scale, 32 * scale,
	                          flags);

	if (window == nullptr) {
		std::cout << "SDL_CreateWindow Error: " << SDL_GetError() << std::endl;
		DestroyContext();
		return false;
	}

	renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
	if (renderer == nullptr) {
		std::cout << "SDL_CreateRenderer Error: " << SDL_GetError() << std::endl;
		DestroyContext();
		return false;
	}

	texture = SDL_CreateTexture(renderer,
	                            SDL_GetWindowPixelFormat(window),
	                            SDL_TEXTUREACCESS_TARGET,
	                            64, 32);

	if (texture == nullptr) {
		std::cout << "SDL_CreateTexture Error: " << SDL_GetError() << std::endl;
		DestroyContext();
	}

	shouldQuit = false;

	return true;
}

void Context::DestroyContext () {
	if (texture)
		SDL_DestroyTexture(texture);
	if (renderer)
		SDL_DestroyRenderer(renderer);
	if (window)
		SDL_DestroyWindow(window);

	SDL_Quit();
}

void Context::HandleEvents () {
	SDL_Event event;

	while (SDL_PollEvent(&event)) {
		if (event.type == SDL_QUIT)
			shouldQuit = true;
		if (event.type == SDL_KEYDOWN && event.key.keysym.sym == SDLK_ESCAPE)
			shouldQuit = true;
	}
}

void Context::Draw (uint8_t displayBuffer[64 * 32]) {
	SDL_SetRenderTarget(renderer, texture);

	SDL_SetRenderDrawColor(renderer, 30, 30, 30, 255);
	SDL_RenderClear(renderer);

	SDL_SetRenderDrawColor(renderer, 210, 210, 210, 255);
	for (int i = 0; i < 32; i++) {
		for (int j = 0; j < 64; j++) {
			if (displayBuffer[i * 64 + j]) {
				SDL_RenderDrawPoint(renderer, j, i);
			}
		}
	}

	SDL_SetRenderTarget(renderer, NULL);
	SDL_RenderCopy(renderer, texture, NULL, NULL);

	SDL_RenderPresent(renderer);
	// HACK: Implement proper framerate control in main loop

}

void Context::Sleep (float ms) {
	SDL_Delay(ms);
}

bool Context::ShouldQuit () {
	return shouldQuit;
}
