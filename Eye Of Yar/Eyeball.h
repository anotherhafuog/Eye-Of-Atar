#ifndef EYEBALL_H
#define EYEBALL_H
#include <SDL.h>
#include <vector>
#include "drawFunctions.h"
#include "GameObject.h"

class Eyeball : public GameEntity {
	public:
		Eyeball();

		~Eyeball();

		void Update(int, SDL_Point);
};

#endif