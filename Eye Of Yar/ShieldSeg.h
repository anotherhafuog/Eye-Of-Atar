#ifndef SHIELDSEG_H
#define SHIELDSEG_H
#include <SDL.h>
#include <vector>
#include "drawFunctions.h"
#include "GameObject.h"

class ShieldSeg : public GameEntity {
private:
	SDL_Rect hitbox;
public:
	ShieldSeg();

	~ShieldSeg();

	const SDL_Rect* getHitbox();
};

#endif
