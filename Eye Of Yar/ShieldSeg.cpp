#include "ShieldSeg.h"
using namespace std;

ShieldSeg::ShieldSeg() {
	vectorData = { {0,10}, {-8,5}, {-8,-5}, {0,-10}, {8, -5}, {8,5} };
	position = { 1280,1000 };
	indices = { 0, 1, 1, 2, 2, 3, 3, 4, 4, 5, 5, 0 };
	vertCt = 6;
	indiceCt = 12;
	scale = 2;
	angle = 0;
	color = { 0,255,255,128 }, //cyan

	hitbox.x = position.x - (6 * scale);
	hitbox.y = position.y - (6 * scale);
	hitbox.w = scale * 12;
	hitbox.h = scale * 12;

	//setting defaults
	DvectorData = vectorData;
	Dposition = position;
	Dindices = indices;
	DvertCt = vertCt;
	DindiceCt = indiceCt;
	Dscale = scale;
	Dangle = angle;
	Dcolor = color;
}

ShieldSeg::~ShieldSeg() {

}

const SDL_Rect* ShieldSeg::getHitbox() {
	return &hitbox;
}