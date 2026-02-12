#include "MagicPixel.h"
using namespace std;

MagicPixel::MagicPixel() {
	vectorData = { {-2,1}, {2,1}, {2,-1}, {-2,-1} };
	position = { 1280,720 };
	indices = { 0, 1, 1, 2, 2, 3, 3, 0};
	vertCt = 4;
	indiceCt = 8;
	scale = 8;
	angle = 0;
	color = colorTable[0];

	hitbox.x = position.x - (2 * scale);
	hitbox.y = position.y - scale;
	hitbox.w = scale * 4;
	hitbox.h = scale * 2;
}

MagicPixel::~MagicPixel() {

}

void MagicPixel::Update(int frameCount, int speed, SDL_Point atarPos) {
	static int i = 0;
	if (i > 7) {
		i = 0;
	}
	else {
		color = colorTable[i];
		i++;
	}
	if (atarPos.x > position.x) {
		position.x += speed;
	}
	else {
		position.x -= speed;
	}
	if (atarPos.y > position.y) {
		position.y += speed;
	}
	else {
		position.y -= speed;
	}

	hitbox.x = position.x - (2 * scale);
	hitbox.y = position.y - scale;
	transformPoints(vectorData, worldData, vertCt, position, scale, angle);
}

const SDL_Rect* MagicPixel::getHitbox() {
	return &hitbox;
}