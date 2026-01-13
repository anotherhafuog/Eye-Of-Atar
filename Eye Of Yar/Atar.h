#ifndef ATAR_H
#define ATAR_H
#include <SDL.h>
#include <vector>
#include "drawFunctions.h"
#include "GameObject.h"

class Atar : public GameEntity {
/*private:
	bool animationState;
	vector<SDL_Point> flapDown;
	vector<SDL_Point> flapUp;*/
public:
	Atar();

	~Atar();

	void Update();
};

#endif
