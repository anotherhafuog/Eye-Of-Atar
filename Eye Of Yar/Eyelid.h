#ifndef EYELID_H
#define EYELID_H
#include <SDL.h>
#include <vector>
#include "drawFunctions.h"
#include "GameObject.h"

class Eyelid : public GameEntity {
private:
	SDL_Rect hitbox;

public:
	Eyelid();

	~Eyelid();

	const SDL_Rect* getHitbox();
};

#endif
