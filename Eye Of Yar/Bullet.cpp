#include "Bullet.h"
using namespace std;

Bullet::Bullet() {
	vectorData = { {-1,1}, {1,1}, {1,-1}, {-1,-1} };
	position = { -1, -1 };
	indices = { 0, 1, 1, 2, 2, 3, 3, 0 };
	vertCt = 4;
	indiceCt = 8;
	scale = 2;
	angle = 0;
	color = { 255,255,255,255 };

	hitbox.x = position.x - scale;
	hitbox.y = position.y - scale;
	hitbox.w = scale * 2;
	hitbox.h = scale * 2;

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

Bullet::~Bullet() {

}

void Bullet::Update(SDL_Point atarPos, string atarDir, const bool pressed[SDL_NUM_SCANCODES]) {
	
	if (pressed[SDL_SCANCODE_SPACE] && (isActive == false)) {
		//shoot from atar pos in right direction
	}

	hitbox.x = position.x - scale;
	hitbox.y = position.y - scale;
	transformPoints(vectorData, worldData, vertCt, position, scale, angle);
}

void Bullet::Deactivate() {
	position = { -100, -100 };
	isActive = false;
}

const SDL_Rect* Bullet::getHitbox() {
	return &hitbox;
}