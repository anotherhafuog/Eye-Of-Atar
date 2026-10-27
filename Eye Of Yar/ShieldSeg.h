#ifndef SHIELDSEG_H
#define SHIELDSEG_H
#include <SDL.h>
#include <vector>
#include "drawFunctions.h"
#include "GameObject.h"

class ShieldSeg : public GameEntity {
private:
	SDL_Rect hitbox;
	int ptGain;
	bool alive;

public:
	ShieldSeg(Uint8 brightness);

	~ShieldSeg();

	void isHit();

	const SDL_Rect* getHitbox();

	void Update();

	bool isAlive() const { return alive; }

	int getPoints() const { return ptGain; }

};

#endif
