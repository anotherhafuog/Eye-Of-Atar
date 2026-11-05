#ifndef SWIRLY_H
#define SWIRLY_H
#include <SDL.h>
#include <vector>
#include "drawFunctions.h"
#include "GameObject.h"
#include "audio.h"

class Swirly : public GameEntity {
private:
	SDL_Rect hitbox;
	bool isActive;
	int speed;
	SDL_Point velocity;
	Mix_Chunk* shoot;
public:
	Swirly();

	~Swirly();

	void Activate(SDL_Point);
	void Update();
	const SDL_Rect* getHitbox();
	void Deactivate();

};

#endif
