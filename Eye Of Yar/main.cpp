#include <SDL.h>
#undef main
#include "drawFunctions.h"
#include "GameObject.h"
#include "Eyeball.h"
#include "Eyelid.h"
#include "Atar.h"
#include "GammaField.h"

using namespace std;

int main() {

	initWindow(2560, 1440, SDL_WINDOW_FULLSCREEN_DESKTOP, 4.0);


	static int frameCount = 0;
	static int gameSpeed = 1;
	struct InputState {
		bool quit = 0;
		bool held[SDL_NUM_SCANCODES];
	};

	InputState input = {};

	Eyeball gameEyeball;
	Eyelid gameEyelid;
	Atar gameAtar;
	GammaField gameGamma(gameSpeed);

	//game loop
	while (!input.quit) {
		Uint32 start = SDL_GetTicks();
		SDL_Event event;
		while (SDL_PollEvent(&event)) {
			switch (event.type) {
			case SDL_QUIT:
				input.quit = true;
				break;
			case SDL_KEYDOWN:
				if (!event.key.repeat) {  // avoid multiple repeats
					input.held[event.key.keysym.scancode] = true;
				}
				break;

			case SDL_KEYUP:
				input.held[event.key.keysym.scancode] = false;
				break;
			}
		}
			gameAtar.Update(frameCount, input.held);
			gameAtar.Render();

			gameEyeball.Update(frameCount);
			gameEyeball.Render();

			gameEyelid.Update();
			gameEyelid.Render();

			gameGamma.Update();
			gameGamma.Render();

			renderFrame();
			clearScreen();
			++frameCount;
			Uint32 elapsed = SDL_GetTicks() - start;
			if (elapsed < 16) {
				SDL_Delay(16 - elapsed);
			}
	}
		killRender();
		return EXIT_SUCCESS;
}