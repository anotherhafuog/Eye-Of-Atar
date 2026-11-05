#include <SDL.h>
#include <SDL_mixer.h>
#include <string>
#undef main
#include "audio.h"
#include "drawFunctions.h"
#include "GameObject.h"
#include "Eyeball.h"
#include "Eyelid.h"
#include "Atar.h"
#include "GammaField.h"
#include "MagicPixel.h"
#include "ShieldSeg.h"
#include "Bullet.h"
#include "ShieldRing.h"
#include "Swirly.h"


using namespace std;
static int frameCount = 0;
static int gameSpeed = 1;
static int level = 0;
int score = 0;
int hiScore = 100000;
int defBonus = 3000;
int bonus = defBonus;
int lives = 3;
bool dead = false;
float atarDirection;
bool bgmStart = 0;
int eyeState = 0;

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
ShieldRing shield;
Bullet gameBullet;

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

void deathInit(SDL_Point choosePos) {
	explosion.localPts = {
		{-2,0},{2,0},{0,2},{0,-2},
		{-1,1},{1,-1},{1,1},{-1,-1}
	};
	explosion.indices = { 0,1,2,3,4,5,6,7 };
	explosion.ptCt = 8;
	explosion.indCt = 8;
	explosion.center = choosePos;
	explosion.scale = 1.0;
	explosion.angle = M_PI / 6;
	explosion.transparency = 255;
	explosion.color = { 255,255,255,explosion.transparency };

	deathAnimCounter = 0;
	
}

int main() {

	initWindow(2560, 1440, SDL_WINDOW_FULLSCREEN_DESKTOP, 4.0);

	//audio initialization
	audio::init();
	Mix_Chunk* medExplode = audio::loadSfx("Sounds/MEDIUM EXPLOSION.mp3");
	Mix_Chunk* zizzer = audio::loadSfx("Sounds/ZIZZER.mp3");
	Mix_Chunk* ping = audio::loadSfx("Sounds/PING.mp3");
	Mix_Chunk* bounce = audio::loadSfx("Sounds/BOUNCE.mp3");
	Mix_Music* bgm = audio::loadMusic("Sounds/SHIP THRUST.mp3"); //todo: lower volume and clean up loop

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
						audio::playSfx(ping);
						SDL_Delay(2000);
						shield.reset(level);
						state = GameState::GAME;//print the speed and delay then continue to main gameplay
					} //convert keystroke into int 1-5
					else if (sc >= SDL_SCANCODE_KP_1 && sc <= SDL_SCANCODE_KP_5) {
						gameSpeed = (sc - SDL_SCANCODE_KP_1) + 1;
						printChar(char('0' + gameSpeed), { 1600, 450 }, { 0, 0, 255, 128 }, 8); //ask for speed
						renderFrame();
						clearScreen();
						audio::playSfx(ping);
						SDL_Delay(2000);
						state = GameState::GAME;//print the speed and delay then continue to main gameplay
					}// for keypad...
					else if (!(sc >= SDL_SCANCODE_1 && sc <= SDL_SCANCODE_5) || !(sc >= SDL_SCANCODE_KP_1 && sc <= SDL_SCANCODE_KP_5)) {
						audio::playSfx(bounce);
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
				audio::playSfx(medExplode);
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
		else if (state == GameState::GAME) {
			Uint32 start = SDL_GetTicks();//for frame update

			if (bgmStart == 0) {
				audio::playMusic(bgm);
				bgmStart = 1;
			}
			//update things
			gameAtar.Update(frameCount, atarDirection, input.held);
			gameAtar.Render();

			SDL_Point tempCheck = gameAtar.getPosition();
			float dx = tempCheck.x - 1280;
			float dy = tempCheck.y - 720;
			float d2 = dx * dx + dy * dy;
			bool inField = (d2 >= (375 * 375) && d2 <= (675 * 675)); //calculate for later use

			gameEyeball.Update(frameCount, gameSpeed, gameAtar.getPosition());
			gameEyeball.Render();

			gameEyelid.Update();
			gameEyelid.Render();

			gameGamma.Update();
			gameGamma.Render();

			gamePixel.Update(frameCount, gameSpeed, gameAtar.getPosition());
			gamePixel.Render();

			shield.Update(1.f / 60.f);
			shield.Render();

			gameBullet.Update(gameAtar.getPosition(), atarDirection, inField, eyeState, input.held);
			gameBullet.Render();

			//display hiscore, score, bonus, lives, and update bonus

			hiScore = (score > hiScore) ? score : hiScore;

			printString(std::to_string(hiScore), { 50, 100 }, { 0, 0, 255, 128 }, 4);
			printString(std::to_string(score), { 2290, 100 }, { 0, 0, 255, 128 }, 4);
			printString(std::to_string(bonus), { 2290, 200 }, { 255, 255, 0, 128 }, 4);
			printString(std::to_string(lives), { 2290, 1340 }, { 0, 0, 255, 128 }, 4);

			//draw screen edges

			drawVectorPic({ {320,0}, {2240, 0}, {2240, 1440}, {320,1440} }, { 0,3,1,2 }, 4, { 0,0,255,32 });

			if ((bonus != 0 && frameCount % 20 == 0)) {
				bonus -= 10;
			}//decrement bonus

			//Collision section: (need to create better handler eventually)

			if (SDL_HasIntersection(gameAtar.getHitbox(), gamePixel.getHitbox()) && !inField) {
				deathInit(gameAtar.getPosition());
				deathAnimCounter = 0;
				audio::playSfx(medExplode);
				state = GameState::DEATH;
			} //hard coded to test death screen for now

			// replace the testSeg / Atar block
			for (int i = 0; i < shield.size(); ++i) {
				if (!shield.alive(i)) continue;
				if (SDL_HasIntersection(gameAtar.getHitbox(), shield.hitbox(i))) {
					SDL_Point atarPos = gameAtar.getPosition();
					SDL_Point eyePos = { 1280, 720 }; //I dont feel like calling getPos!
					int dx = atarPos.x - eyePos.x;
					int dy = atarPos.y - eyePos.y;
					float length = sqrt(dx * dx + dy * dy);
					float ux = dx / length;
					float uy = dy / length;
					gameAtar.addPosition({ static_cast<int>(std::lround(ux * 32)), static_cast<int>(std::lround(uy * 32)) }); //may increase bump distance later
					break;                    // bump once per frame
				}
			}

			if (SDL_HasIntersection(gameAtar.getHitbox(), gameEyeball.getHitbox()) && eyeState == 0)
			{
				eyeState = 2;
				gameEyeball.changeState(2);
			}

			// replace the testSeg / bullet block
			for (int i = 0; i < shield.size(); ++i) {
				if (!shield.alive(i)) continue;
				if (SDL_HasIntersection(gameBullet.getHitbox(), shield.hitbox(i))) {
					gameBullet.Deactivate();
					score += shield.hit(i);
					audio::playSfx(zizzer);
					break;                    // one bullet, one segment
				}
			}

			renderFrame();
			clearScreen();
			frameCount = (frameCount + 1) % 60;
			Uint32 elapsed = SDL_GetTicks() - start;
			if (elapsed < 16) {
				SDL_Delay(16 - elapsed);
			}//render frame and update counters
		}

		else if (state == GameState::DEATH) {
			Uint32 start = SDL_GetTicks();//for frame update
			if (deathAnimCounter < 200) {

				transformPoints(explosion.localPts, explosion.worldPts, explosion.ptCt, explosion.center, explosion.scale, explosion.angle);
				drawVectorPic(explosion.worldPts, explosion.indices, explosion.indCt, explosion.color);

				if (deathAnimCounter % 10) {
					explosion.scale++;
				}

				gameEyeball.Render();

				gameEyelid.Update();
				gameEyelid.Render();

				gameGamma.Update();
				gameGamma.Render();

				shield.Update(1.0f / 60.0f);
				shield.Render();
				//display hiscore, score, bonus, lives

				printString(std::to_string(hiScore), { 50, 100 }, { 0, 0, 255, 128 }, 4);
				printString(std::to_string(score), { 2290, 100 }, { 0, 0, 255, 128 }, 4);
				printString(std::to_string(bonus), { 2290, 200 }, { 255, 255, 0, 128 }, 4);
				printString(std::to_string(lives), { 2290, 1340 }, { 0, 0, 255, 128 }, 4);

				drawVectorPic({ {320,0}, {2240, 0}, {2240, 1440}, {320,1440} }, { 0,3,1,2 }, 4, { 0,0,255,32 });

				explosion.color = { 255, 255, 255, --explosion.transparency };
			}
			else if (lives - 1 == -1) {
				bgmStart = 0;
				audio::stopMusic();
				deathAnimCounter = 0;
				state = GameState::GAMEOVER;
			}
			else {
				//bgmStart = 0;
				//audio::stopMusic();
				printString(std::to_string(score), { 1600, 600 }, { 0, 0, 255, 128 }, 8);
				printString(std::to_string(lives - 1), { 1600, 450 }, { 0, 0, 255, 128 }, 8);

				if (deathAnimCounter > 380) {
					--lives; //should happen once
					gameAtar.reset();
					gamePixel.reset();
					gameEyeball.reset();
					gameBullet.Deactivate();
					gameBullet.reset();
					state = GameState::GAME;
				}
			}

			++deathAnimCounter;

			renderFrame();
			clearScreen();
			frameCount = (frameCount + 1) % 60;

			Uint32 elapsed = SDL_GetTicks() - start;
			if (elapsed < 16) {
				SDL_Delay(16 - elapsed);
			}//render frame and update counters
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
				shield.reset(level);
				gameBullet.Deactivate();
				gameBullet.reset();
				clearScreen();
				deathAnimCounter = 0;
				state = GameState::TITLE;
			}
		}

		else if (state == GameState::WIN) {
			Uint32 start = SDL_GetTicks();//for frame update
			if (deathAnimCounter < 200) {

				transformPoints(explosion.localPts, explosion.worldPts, explosion.ptCt, explosion.center, explosion.scale, -explosion.angle);
				drawVectorPic(explosion.worldPts, explosion.indices, explosion.indCt, explosion.color);

				if (deathAnimCounter % 10) {
					explosion.scale++;
				}

				gameEyelid.Update();
				gameEyelid.Render();

				gameGamma.Update();
				gameGamma.Render();

				shield.Update(1.0f / 60.0f);
				shield.Render();
				//display hiscore, score, bonus, lives

				printString(std::to_string(hiScore), { 50, 100 }, { 0, 0, 255, 128 }, 4);
				printString(std::to_string(score), { 2290, 100 }, { 0, 0, 255, 128 }, 4);
				printString(std::to_string(bonus), { 2290, 200 }, { 255, 255, 0, 128 }, 4);
				printString(std::to_string(lives), { 2290, 1340 }, { 0, 0, 255, 128 }, 4);

				explosion.color = { 255, 255, 0, --explosion.transparency };
			}

			else {
				//bgmStart = 0;
				//audio::stopMusic();
				score += bonus;
				bonus = defBonus;
				printString(std::to_string(score), { 1600, 600 }, { 0, 0, 255, 128 }, 8);
				printString(std::to_string(lives - 1), { 1600, 450 }, { 0, 0, 255, 128 }, 8);

				if (deathAnimCounter > 380) {
					gameSpeed = (gameSpeed < 5) ? gameSpeed++ : gameSpeed;
					level = (level + 1) % 2;
					gameAtar.reset();
					gamePixel.reset();
					gameEyeball.reset();
					gameBullet.Deactivate();
					gameBullet.reset();
					shield.reset(level);
					//todo: make gamma rings reset
					state = GameState::GAME;
				}
			}

			++deathAnimCounter;

			renderFrame();
			clearScreen();
			frameCount = (frameCount + 1) % 60;

			Uint32 elapsed = SDL_GetTicks() - start;
			if (elapsed < 16) {
				SDL_Delay(16 - elapsed);
			}//render frame and update counters
			}
	}

	
	//freeing audio
	Mix_FreeChunk(medExplode);
	Mix_FreeChunk(ping);
	Mix_FreeChunk(zizzer);
	Mix_FreeChunk(bounce);
	Mix_FreeMusic(bgm);
	audio::shutdown();

	killRender();
	return EXIT_SUCCESS;
}

