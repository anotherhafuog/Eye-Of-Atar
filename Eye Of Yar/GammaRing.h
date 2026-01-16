#ifndef GAMMARING_H
#define GAMMARING_H
#include <SDL.h>
#include <vector>
#include <cmath>
#include <time.h>
#include "drawFunctions.h"
#include "GameObject.h"

class GammaRing : public GameEntity {
private:
	int speed;
public:
	GammaRing(int, float, SDL_Color);

	~GammaRing();

	void Update();
};

#endif