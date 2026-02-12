#include <SDL.h>
#include <string>
#undef main
#include "drawFunctions.h"
#include "GameObject.h"
#include "Eyeball.h"
#include "Eyelid.h"
#include "Atar.h"
#include "GammaField.h"
#include "MagicPixel.h"

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
	enum class GameState {
		SELECT,
		GAME,
		DEATH,
		WIN,
		GAMEOVER
	};
	GameState state = GameState::SELECT;

	Eyeball gameEyeball;
	Eyelid gameEyelid;
	Atar gameAtar;
	GammaField gameGamma(gameSpeed);
	MagicPixel gamePixel;

	int score = 0;
	int hiScore = 10000;
	int bonus = 3000;
	int lives = 4;
	bool dead = false;

	//game loop
	while (!input.quit) {
		//poll keyboard events (joystick maybe someday)
		SDL_Event event;
		while (SDL_PollEvent(&event)) {
			SDL_Scancode sc = event.key.keysym.scancode;
			switch (event.type) {
			case SDL_QUIT:
				input.quit = true;
				break;
			case SDL_KEYDOWN:
				if (!event.key.repeat) {  // avoid multiple repeats
					input.held[sc] = true;
				}
				if (state == GameState::SELECT) {
					if (sc >= SDL_SCANCODE_1 && sc <= SDL_SCANCODE_5) {
						gameSpeed = (sc - SDL_SCANCODE_1) + 1;
						printChar(char('0' + gameSpeed), { 1600, 450 }, { 0, 0, 255, 128 }, 8); //ask for speed
						renderFrame();
						clearScreen();
						SDL_Delay(2000);
						state = GameState::GAME;//print the speed and delay then continue to main gameplay
					} //convert keystroke into int 1-5
					else if (sc >= SDL_SCANCODE_KP_1 && sc <= SDL_SCANCODE_KP_5) {
						gameSpeed = (sc - SDL_SCANCODE_KP_1) + 1;
						printChar(char('0' + gameSpeed), { 1600, 450 }, { 0, 0, 255, 128 }, 8); //ask for speed
						renderFrame();
						clearScreen();
						SDL_Delay(2000);
						state = GameState::GAME;//print the speed and delay then continue to main gameplay
					}// for keypad...
					else {
						//make bad noise when i have sound
					}
				}
				else if (state == GameState::DEATH) {
					//wait for input then reset to initial game state
				}
				break;

			case SDL_KEYUP:
				input.held[sc] = false;
				break;
			}
		}

		//speed select (wip very rudimentary now)
		if (state == GameState::SELECT) {
			printChar('?', { 1600, 450 }, { 0, 0, 255, 128 }, 8); //ask for speed
			renderFrame();
			clearScreen();
		}

		// main gameplay loop
		else if (state == GameState::GAME){
			Uint32 start = SDL_GetTicks();//for frame update
			//update things
			gameAtar.Update(frameCount, input.held);
			gameAtar.Render();

			gameEyeball.Update(frameCount, gameAtar.getPosition());
			gameEyeball.Render();

			gameEyelid.Update();
			gameEyelid.Render();

			gameGamma.Update();
			gameGamma.Render();

			gamePixel.Update(frameCount, gameSpeed, gameAtar.getPosition());
			gamePixel.Render();

			//display hiscore, score, bonus, lives, and update bonus

			printString(std::to_string(hiScore), { 100, 100 }, { 0, 0, 255, 128 }, 4);
			printString(std::to_string(score), { 2290, 100 }, { 0, 0, 255, 128 }, 4);
			printString(std::to_string(bonus), { 2290, 200 }, { 255, 255, 0, 128 }, 4);
			printString(std::to_string(lives), { 2290, 1340 }, { 0, 0, 255, 128 }, 4);

			if ((bonus != 0 && frameCount % 10 == 0)) {
				bonus -= 10;
			}//decrement bonus

			if (SDL_HasIntersection(gameAtar.getHitbox(), gamePixel.getHitbox())) {
				cout << "colliding\n";
				state = GameState::DEATH;
			} //to test death screen for now

			renderFrame();
			clearScreen();
			++frameCount;
			Uint32 elapsed = SDL_GetTicks() - start;
			if (elapsed < 16) {
				SDL_Delay(16 - elapsed);
			}//render frame and update counters
		}
		else if (state == GameState::DEATH) {
			SDL_Point explosionPos = gameAtar.getPosition();
			unsigned int animationCounter = 0;
			struct{
				vector<SDL_Point> localPts = { {-2,0}, {2,0}, {0,2}, {0,-2}, {-1,1}, {1,-1}, {1,1}, {-1,-1} };//temporary
				vector<SDL_Point> worldPts;
				vector<int> indices = { 0,1,2,3,4,5,6,7 };
				int ptCt = 8;
				int indCt = 8;
				SDL_Point center = { 0,0 };
				float scale = 1;
				float angle = 0;
				SDL_Color color = { 255,255,255,255 };
			}explosion;
			explosion.center = gameAtar.getPosition();
			if (animationCounter < 300) {
				Uint32 start = SDL_GetTicks();//for frame update

				transformPoints(explosion.localPts,explosion.worldPts,explosion.ptCt,explosion.center,explosion.scale, explosion.angle);
				drawVectorPic(explosion.worldPts, explosion.indices, explosion.indCt, explosion.color);

				if (animationCounter % 10) {
					explosion.scale++;
				}

				gameEyeball.Render();

				gameEyelid.Update();
				gameEyelid.Render();

				gameGamma.Update();
				gameGamma.Render();

				//display hiscore, score, bonus, lives

				printString(std::to_string(hiScore), { 100, 100 }, { 0, 0, 255, 128 }, 4);
				printString(std::to_string(score), { 2290, 100 }, { 0, 0, 255, 128 }, 4);
				printString(std::to_string(bonus), { 2290, 200 }, { 255, 255, 0, 128 }, 4);
				printString(std::to_string(lives), { 2290, 1340 }, { 0, 0, 255, 128 }, 4);

				++animationCounter;

				renderFrame();
				clearScreen();
				++frameCount;
				Uint32 elapsed = SDL_GetTicks() - start;
				if (elapsed < 16) {
					SDL_Delay(16 - elapsed);
				}//render frame and update counters
			}

			--lives;
			printChar(char('0' + score), { 1400, 400 }, { 0, 0, 255, 128 }, 8); //ask for score
			printChar(char('0' + lives), { 1600, 450 }, { 0, 0, 255, 128 }, 8); //ask for lives

		}
	}
		killRender();
		return EXIT_SUCCESS;
}