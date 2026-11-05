#ifndef BULLET_H
#define BULLET_H
#include <SDL.h>
#include <vector>
#include "drawFunctions.h"
#include "GameObject.h"
#include "audio.h"

class Bullet : public GameEntity {
private:
	SDL_Rect hitbox;
	bool isActive;
	SDL_Point velocity;
	int speed = 20;
	Mix_Chunk* shoot;

public:
	Bullet();

	~Bullet();

	void Update(SDL_Point, float, bool, int, const bool pressed[SDL_NUM_SCANCODES]); //uhh
	void Deactivate();
	const SDL_Rect* getHitbox();
};

#endif