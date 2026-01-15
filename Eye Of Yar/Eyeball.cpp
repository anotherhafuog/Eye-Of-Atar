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
	color = { 255, 255, 0, 128 };
}

Eyeball::~Eyeball() {

}

void Eyeball::Update(int frameCount) {
	transformPoints(vectorData, worldData, vertCt, position, scale, angle);
}