#ifndef MAGICPIXEL_H
#define MAGICPIXEL_H
#include <SDL.h>
#include <vector>
#include "drawFunctions.h"
#include "GameObject.h"

class MagicPixel : public GameEntity {
private:
	SDL_Color colorTable[7] =
	{
		{0,0,255,128}, //blue
		{0,255,0,128}, //green
		{0,255,255,128}, //cyan
		{255,0,0,128}, // red
		{255,0,255,128}, //pink
		{255,255,0,128}, //yellow
		{255,255,255,128} //white
	};
public:
	MagicPixel();

	~MagicPixel();

	void Update(int, int, SDL_Point);
};

#endif