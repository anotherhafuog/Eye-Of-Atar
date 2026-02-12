#ifndef ATAR_H
#define ATAR_H
#include <SDL.h>
#include <vector>
#include <cmath>
#include "drawFunctions.h"
#include "GameObject.h"

class Atar : public GameEntity {
private:
	bool animationState;
	std::vector<SDL_Point> flapDown;
	std::vector<SDL_Point> flapUp;
	SDL_Rect hitbox;
public:
	Atar();

	~Atar();

	void Update(int, const bool pressed[SDL_NUM_SCANCODES]);

	const SDL_Rect* getHitbox();
};

#endif
