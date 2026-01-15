#include "Atar.h"
using namespace std;

Atar::Atar() {
	vector<SDL_Point> flapUp =
	{ {-3, -5}, { -2,-6 }, { 2,-6 }, { 3,-5 }, { 3,-1 }, { 2,0 }, { -2,0 }, { -3,-1 }, /*head*/
		{ -2,7 }, { 2,7 }, { 4,9 }, { -4,9 }, /*tail*/
		{ -3,-7 }, { -2,-9 }, { -1,-7 }, /*antenna left*/
		{ 1,-7 }, { 2,-9 }, { 3,-7 }, /*antenna right*/
		{ -7,1 }, { -10,0 }, { -8,-8 }, { -3,-1 }, /*wing left*/
		{ 7,1 }, { 10,0 }, { 8,-8 }, { 3,-1 } /*wing right*/
	};
	vector<SDL_Point> flapDown =
	{ {-3,-5}, {-2,-6}, {2,-6}, {3,-5}, {3,-1}, {2,0}, {-2,0}, {-3,-1}, /*head*/
		{-2,7}, {2,7}, {4,9}, {-4,9}, /*tail*/
		{-3,-7}, {-2,-9}, {-1,-7}, /*antenna left*/
		{1,-7}, {2,-9}, {3,-7}, /*antenna right*/
		{-7,1}, {-10,4}, {-8,8}, {-2,1}, /*wing left*/
		{7,1}, {10,4}, {8,8}, {2,1} /*wing right*/
	};
	vectorData = flapDown;
	position = { 200,720 };
	indices = 
	{	0, 1, 1, 2, 2, 3, 3, 4, 4, 5, 5, 6, 6, 7, 7, 0, /*head*/
		8, 6, 8, 9, 9, 5, /*body*/
		9, 10, 10, 11, 11, 8, /*tail*/
		1, 12, 12, 13, 13, 14, 14, 1, /*antenna left*/
		2, 15, 15, 16, 16, 17, 17, 2, /*antenna right*/
		6, 18, 18, 19, 19, 20, 20, 21, /*wing left*/
		5, 22, 22, 23, 23, 24, 24, 25
	};
	vertCt = 26;
	indiceCt = 60;
	scale = 4;
	angle = 0;
	color = { 255, 0, 255, 128 };
	//animationState = 0;
}

Atar::~Atar() {

}

void Atar::Update(int frameCount, const bool pressed[SDL_NUM_SCANCODES]) {
	int speed = 10;
	if (pressed[SDL_SCANCODE_W]) {
		if (pressed[SDL_SCANCODE_A]) {
			angle = -3.14 / 4;
			position.x -= speed;
		}
		else if (pressed[SDL_SCANCODE_D]) {
			angle = 3.14 / 4;
			position.x += speed;
		}
		else{
			angle = 0;
		}
		position.y -= speed;
	}
	else if (pressed[SDL_SCANCODE_S]) {
		if (pressed[SDL_SCANCODE_A]) {
			angle = -3*3.14 / 4;
			position.x -= speed;
		}
		else if (pressed[SDL_SCANCODE_D]) {
			angle = 3*3.14 / 4;
			position.x += speed;
		}
		else {
			angle = 3.14;
		}
		position.y += speed;
	}
	else if (pressed[SDL_SCANCODE_A]) {
		angle = -3.14 / 2;
		position.x -= speed;
	}
	else if (pressed[SDL_SCANCODE_D]) {
		angle = 3.14 / 2;
		position.x += speed;
	}
	else {

	}
	/*switch (animationState) {
	case 0:
		vectorData = flapDown;
		break;
	case 1:
		vectorData = flapUp;
	}
	animationState = !animationState;*/
	transformPoints(vectorData, worldData, vertCt, position, scale, angle);
}