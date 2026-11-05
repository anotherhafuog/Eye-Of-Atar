#ifndef EYEBALL_H
#define EYEBALL_H
#include <SDL.h>
#include <vector>
#include <cmath>
#include <time.h>
#include "drawFunctions.h"
#include "GameObject.h"
#include "audio.h"

class Eyeball : public GameEntity {
private:
	enum class EyeState {
		IDLE, //0
		BLINKING, //1
		BOMB, //2
		TRAVELING, //3
		DANGEROUS //4
	};
	EyeState state;
	SDL_Rect hitbox;
	bool isActive;
	SDL_Point velocity;
	Mix_Chunk* zoom;
	bool firing;

	SDL_FPoint fpos;
	SDL_FPoint fvel;

	int blinkFrames;   // last time the eye toggled open/closed
	int blinksLeft;
	bool eyeClosed;      // for your draw code to read

	std::vector<SDL_Point> BvectorData;
	std::vector<int> Bindices;
	int BvertCt;
	int BindiceCt;
	SDL_Color Bcolor;

	public:
		Eyeball();

		~Eyeball();

		void Update(int, int, SDL_Point, const bool pressed[SDL_NUM_SCANCODES]);
		void changeState(int);
		void reset();
		const SDL_Rect* getHitbox();
		int getState();
		bool isFiring();
		void closeEye();
		void openEye();
};

#endif