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
	color = { 0, 0, 0, 0 };

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

void Bullet::Update(SDL_Point atarPos, float atarDir, const bool pressed[SDL_NUM_SCANCODES]) {
	
	if (pressed[SDL_SCANCODE_SPACE] && !isActive) {
		isActive = true;
		position = atarPos;
		velocity.x = (int)std::lround(std::sin(atarDir) * speed);
		velocity.y = (int)std::lround(-std::cos(atarDir) * speed);
		color = { 255,255,255,255 };
	}
	if (isActive) {
		position.x += velocity.x;
		position.y += velocity.y;

		if (position.x + scale < 320 || position.x - scale > 2240 ||
			position.y + scale < 0 || position.y - scale > 1440) {
			Deactivate();
		}
		//shoot from atar pos in right direction
	}

	hitbox.x = position.x - scale;
	hitbox.y = position.y - scale;
	transformPoints(vectorData, worldData, vertCt, position, scale, angle);
}

void Bullet::Deactivate() {
	position = { -100, -100 };
	color = { 0, 0, 0, 0 };
	isActive = false;
}

const SDL_Rect* Bullet::getHitbox() {
	return &hitbox;
}