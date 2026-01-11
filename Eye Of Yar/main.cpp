#include <SDL.h>
#undef main
#include "drawFunctions.h"
#include "GameObject.h"
#include "Eyeball.h"
#include "Eyelid.h"

using namespace std;

int main() {

	initWindow(2560, 1440, SDL_WINDOW_FULLSCREEN_DESKTOP, 4.0);
	//defines colors (r,g,b, alpha)
	SDL_Color halfBlue = { 0, 0, 255, 128 }; //todo: make color table and brightness manip functions
	SDL_Color halfRed = { 255, 0, 0, 128 }; 
	SDL_Color halfWhite = { 255, 255, 255, 128 };

	Eyeball gameEyeball;
	Eyelid gameEyelid;

	//game loop
	bool running = 1;
	while (running) {
		Uint32 start = SDL_GetTicks();
		SDL_Event event;
		while(SDL_PollEvent(&event)){
			//handle each event
			if (event.type == SDL_QUIT) {
				running = false;
			}
			if (event.key.keysym.sym == SDLK_a) {
				cout << "a is pressed\n";
			}
		}
		gameEyeball.Update();
		gameEyeball.Render();
		gameEyelid.Update();
		gameEyelid.Render();

		renderFrame();
		clearScreen();
		Uint32 elapsed = SDL_GetTicks()-start;
		if (elapsed < 16) {
			SDL_Delay(16 - elapsed);
		}
	}
	
	killRender();
	return EXIT_SUCCESS;
}