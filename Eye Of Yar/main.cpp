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
#include "ShieldSeg.h"

using namespace std;

static int frameCount = 0;
static int gameSpeed = 1;
struct InputState {
	bool quit = 0;
	bool held[SDL_NUM_SCANCODES];
};

InputState input = {};
enum class GameState {
	TITLE,
	SELECT,
	GAME,
	DEATH,
	WIN,
	GAMEOVER
};
GameState state = GameState::TITLE;

Eyeball gameEyeball;
Eyelid gameEyelid;
Atar gameAtar;
GammaField gameGamma(gameSpeed);
MagicPixel gamePixel;
ShieldSeg testSeg;

int score = 0;
int hiScore = 10000;
int bonus = 3000;
int lives = 3;
bool dead = false;

//death defines
struct Explosion {
	std::vector<SDL_Point> localPts;
	std::vector<SDL_Point> worldPts;
	std::vector<int> indices;
	int ptCt;
	int indCt;
	SDL_Point center;
	float scale;
	float angle;
	Uint8 transparency;
	SDL_Color color;
};

Explosion explosion;
unsigned int deathAnimCounter = 0;

void deathInit(SDL_Point atarPos) {
	explosion.localPts = {
		{-2,0},{2,0},{0,2},{0,-2},
		{-1,1},{1,-1},{1,1},{-1,-1}
	};
	explosion.indices = { 0,1,2,3,4,5,6,7 };
	explosion.ptCt = 8;
	explosion.indCt = 8;
	explosion.center = atarPos;
	explosion.scale = 1.0;
	explosion.angle = M_PI / 6;
	explosion.transparency = 255;
	explosion.color = { 255,255,255,explosion.transparency };

	deathAnimCounter = 0;
	
}

int main() {

	initWindow(2560, 1440, SDL_WINDOW_FULLSCREEN_DESKTOP, 4.0);

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
				if (state == GameState::TITLE) {
					if (sc == SDL_SCANCODE_SPACE) {
						state = GameState::SELECT;
					}
				}
				else if (state == GameState::SELECT) {
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
					if (deathAnimCounter >= 200) {
						if (sc == SDL_SCANCODE_SPACE) {
							--lives; //should happen once
							gameAtar.reset();
							gamePixel.reset();
							gameEyeball.reset();
							testSeg.reset();
							state = GameState::GAME;
						}
					}
				}
				break;

			case SDL_KEYUP:
				input.held[sc] = false;
				break;
			}
		}

		if (state == GameState::TITLE) {
			if (deathAnimCounter == 0) {
				printString("EYE OF ATAR", { 848,570 }, { 255, 255, 255, 255 }, 8);
				printString("MMXXVI PROMETHEUS", { 944,1224 }, { 0, 0, 255, 64 }, 4);
				renderFrame();
				deathAnimCounter = 1;
			}
		}

		//speed select (wip very rudimentary now)
		else if (state == GameState::SELECT) {
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

			testSeg.Update();
			testSeg.Render();

			//display hiscore, score, bonus, lives, and update bonus

			printString(std::to_string(hiScore), { 100, 100 }, { 0, 0, 255, 128 }, 4);
			printString(std::to_string(score), { 2290, 100 }, { 0, 0, 255, 128 }, 4);
			printString(std::to_string(bonus), { 2290, 200 }, { 255, 255, 0, 128 }, 4);
			printString(std::to_string(lives), { 2290, 1340 }, { 0, 0, 255, 128 }, 4);

			if ((bonus != 0 && frameCount % 10 == 0)) {
				bonus -= 10;
			}//decrement bonus

			/*if (SDL_HasIntersection(gameAtar.getHitbox(), gamePixel.getHitbox())) {
				deathInit(gameAtar.getPosition());
				deathAnimCounter = 0;
				state = GameState::DEATH;
			} //hard coded to test death screen for now*/

			if (SDL_HasIntersection(gameAtar.getHitbox(), testSeg.getHitbox())) {
				SDL_Point atarPos = gameAtar.getPosition();
				SDL_Point eyePos = { 1280, 720 }; //I dont wanna call getPos!
				int dx = atarPos.x - eyePos.x;
				int dy = atarPos.y - eyePos.y;
				float length = sqrt(dx * dx + dy * dy);
				float ux = dx / length;
				float uy = dy / length;
				gameAtar.Move({ (int)ux * 100, (int)uy * 100 });
			}//todo: test fun collision

			renderFrame();
			clearScreen();
			frameCount = (frameCount + 1) % 60;
			Uint32 elapsed = SDL_GetTicks() - start;
			if (elapsed < 16) {
				SDL_Delay(16 - elapsed);
			}//render frame and update counters
		}
		else if (state == GameState::DEATH) {
			
			if (deathAnimCounter < 200) {
				Uint32 start = SDL_GetTicks();//for frame update

				transformPoints(explosion.localPts,explosion.worldPts,explosion.ptCt,explosion.center,explosion.scale, explosion.angle);
				drawVectorPic(explosion.worldPts, explosion.indices, explosion.indCt, explosion.color);

				if (deathAnimCounter % 10) {
					explosion.scale++;
				}

				gameEyeball.Render();

				gameEyelid.Update();
				gameEyelid.Render();

				gameGamma.Update();
				gameGamma.Render();

				testSeg.Render();
				//display hiscore, score, bonus, lives

				printString(std::to_string(hiScore), { 100, 100 }, { 0, 0, 255, 128 }, 4);
				printString(std::to_string(score), { 2290, 100 }, { 0, 0, 255, 128 }, 4);
				printString(std::to_string(bonus), { 2290, 200 }, { 255, 255, 0, 128 }, 4);
				printString(std::to_string(lives), { 2290, 1340 }, { 0, 0, 255, 128 }, 4);

				explosion.color = {255, 255, 255, --explosion.transparency};
				++deathAnimCounter;

				renderFrame();
				clearScreen();
				frameCount = (frameCount + 1) % 60;
				Uint32 elapsed = SDL_GetTicks() - start;
				if (elapsed < 16) {
					SDL_Delay(16 - elapsed);
				}//render frame and update counters
			}
			else if (lives-1 == -1) {
				deathAnimCounter = 0;
				state = GameState::GAMEOVER;
			}
			else {
				clearScreen();
				printString(std::to_string(score), { 1600, 600 }, { 0, 0, 255, 128 }, 8);
				printString(std::to_string(lives-1), { 1600, 450 }, { 0, 0, 255, 128 }, 8);
				renderFrame();
			}
		}
		else if (state == GameState::GAMEOVER) {
			if (deathAnimCounter < 300) {
				Uint32 start = SDL_GetTicks();//for frame update
				
				printString("GAME OVER", { 928,672 }, { 255, 0, 0, 255 }, 8);
				renderFrame();
				clearScreen();

				++deathAnimCounter;
				Uint32 elapsed = SDL_GetTicks() - start;
				if (elapsed < 16) {
					SDL_Delay(16 - elapsed);
				}//render frame and update counters
			} //wait for 5 seconds without impeding ability to exit game
			else {
				lives = 3;
				score = 0;
				bonus = 3000;
				gameAtar.reset();
				gamePixel.reset();
				gameEyeball.reset();
				testSeg.reset();
				clearScreen();
				deathAnimCounter = 0;
				state = GameState::TITLE;
			}
		}
	}
		killRender();
		return EXIT_SUCCESS;
}

