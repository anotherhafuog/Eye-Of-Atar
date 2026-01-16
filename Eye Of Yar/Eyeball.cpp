#include "Eyeball.h"
using namespace std;

Eyeball::Eyeball() {
	vectorData = { {-1,1}, {1,1}, {-1,1}, {-1,-1}, {1,-1}, {1,1} };
	position = { 1280,720 };
	indices = { 0, 1, 2, 3, 3, 4, 4, 5 }; //important to have 6 for eye closing animation
	vertCt = 6;
	indiceCt = 8;
	scale = 16;
	angle = 0;
	color = { 255, 255, 0, 255 };
}

Eyeball::~Eyeball() {

}

void Eyeball::Update(int frameCount, SDL_Point atarPos){
	if (atarPos.x > position.x) {
		position.x += 1;
	}
	else {
		position.x -= 1;
	}
	if (atarPos.y > position.y) {
		position.y += 1;
	}
	else {
		position.y -= 1;
	}

	float radius = 10.0;
	SDL_Point d;
	d.x = position.x - 1280;
	d.y = position.y - 720;

	float distance = std::sqrt(
		static_cast<float>(d.x * d.x + d.y * d.y)
	); //get distance away from center

	if (distance > radius && distance != 0.0f) {
		position.x = 1280 +
			static_cast<int>((d.x / distance) * radius);

		position.y = 720 +
			static_cast<int>((d.y / distance) * radius);
	} //make position be inside the circle
	//temporary until i figure out collision

	transformPoints(vectorData, worldData, vertCt, position, scale, angle);
}