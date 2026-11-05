#include "Swirly.h"
using namespace std;

Swirly::Swirly() {
	vectorData = { {0,-1}, {0,1}, {-1,0}, {1,0}, {1,1}, {-1,-1}, {-1,1}, {1,-1}, {-1,-2}, {1,-2}, {2,-1}, {2,1}, {1,2}, {2,-1}, {1,-2}, {-2,-1} };
	position = { 1280,720 };
	indices = { 0, 1, 2, 3, 4, 5, 6, 7, 0, 8, 7, 9, 3, 10, 4, 11, 1, 12, 6, 13, 2, 14, 5, 15};
	vertCt = 16;
	indiceCt = 24;
	scale = 16;
	angle = 0;
	color = { 255,0,0,128 };

	hitbox.x = position.x - (2 * scale);
	hitbox.y = position.y - (2 * scale);
	hitbox.w = scale * 4;
	hitbox.h = scale * 4;

	//setting defaults
	DvectorData = vectorData;
	Dposition = position;
	Dindices = indices;
	DvertCt = vertCt;
	DindiceCt = indiceCt;
	Dscale = scale;
	Dangle = angle;
	Dcolor = color;

	speed = 10;
	isActive = 0;
	velocity = { 0,0 };
	audio::init();
	shoot = audio::loadSfx("Sounds/FIREBALL.mp3");
}

Swirly::~Swirly() {
	Mix_FreeChunk(shoot);
	audio::shutdown();
}

void Swirly::Activate(SDL_Point atarPos) {
	isActive = 1;
	color = { 255,0,0,128 };
	float dx = atarPos.x - 1280;
	float dy = atarPos.y - 720;
	float len = std::sqrt(dx * dx + dy * dy);

	if (len > 0.0001f) {          // avoid divide by zero
		velocity.x = (dx / len) * speed;  // speed in pixels/second
		velocity.y = (dy / len) * speed;
	}
	else {
		velocity.x = velocity.y = 0;
	}
}

void Swirly::Update() {
	if (isActive) {
		position.x += velocity.x;
		position.y += velocity.y;

		if (position.x + scale < 320 || position.x - scale > 2240 ||
			position.y + scale < 0 || position.y - scale > 1440) {
			Deactivate();
		}
		//shoot from atar pos in right direction
	}

	hitbox.x = position.x - (2 * scale);
	hitbox.y = position.y - (2 * scale);
	angle = (angle < 360) ? angle++ : 0;
	transformPoints(vectorData, worldData, vertCt, position, scale, angle);
}

const SDL_Rect* Swirly::getHitbox() {
	return &hitbox;
}

void Swirly::Deactivate() {
	position = { -100, -100 };
	color = { 0, 0, 0, 0 };
	isActive = false;
}