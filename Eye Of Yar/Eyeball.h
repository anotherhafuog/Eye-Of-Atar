#ifndef EYEBALL_H
#define EYEBALL_H
#include <SDL.h>
#include <vector>
#include <cmath>
#include <time.h>
#include "drawFunctions.h"
#include "GameObject.h"

class Eyeball : public GameEntity {
private:
	enum class EyeState {
		IDLE,
		BLINKING,
		BOMB,
		TRAVELING,
		DANGEROUS
	};
	EyeState state;
	SDL_Rect hitbox;

	public:
		Eyeball();

		~Eyeball();

		void Update(int, int, SDL_Point);
		void changeState(int);
		void reset();
		const SDL_Rect* getHitbox();
};

#endif