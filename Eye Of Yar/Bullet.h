#ifndef BULLET_H
#define BULLET_H
#include <SDL.h>
#include <vector>
#include "drawFunctions.h"
#include "GameObject.h"

class Bullet : public GameEntity {
private:
	SDL_Rect hitbox;
	bool isActive;
public:
	Bullet();

	~Bullet();

	void Update(SDL_Point, string, const bool pressed[SDL_NUM_SCANCODES]); //uhh
	void Deactivate();
	const SDL_Rect* getHitbox();
};

#endif