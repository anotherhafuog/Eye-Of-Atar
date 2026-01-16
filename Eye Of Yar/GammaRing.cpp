#include "GammaRing.h"
using namespace std;

GammaRing::GammaRing(int ringSpeed, float ringScale, SDL_Color ringColor) {
	vectorData.clear();
	vectorData.reserve(32);

	constexpr int N = 32;
	constexpr double TWO_PI = 6.283185307179586;

	srand(time(NULL));// for rand seeding

	for (int i = 0; i < N; i++)
	{
		double theta = TWO_PI * i / N;
		int rand1 = 0;
		int rand2 = 0;

		if (i % 2 == 0) {
			rand1 = (std::rand() % (static_cast<int>(ringScale) / 4) - (static_cast<int>(ringScale) / 8));
			rand2 = (std::rand() % (static_cast<int>(ringScale) / 4) - (static_cast<int>(ringScale) / 8));
		}

		int x = static_cast<int>(std::lround(ringScale * std::cos(theta)) + rand1);
		int y = static_cast<int>(std::lround(ringScale * std::sin(theta)) + rand2);

		vectorData.push_back({ x, y });
	}//generates a 32gon for me

	position = { 1280,720 };
	indices = { 0, 1, 1, 2, 2, 3, 3, 4, 4, 5, 5, 6, 6, 7, 7, 8, 8, 9, 9, 10, 10, 11, 11, 12, 12, 13, 13, 14, 14, 15, 15,
		16, 16, 17, 17, 18, 18, 19, 19, 20, 20, 21, 21, 22, 22, 23, 23, 24, 24, 25, 25, 26, 26, 27, 27, 28, 28, 29, 29, 30, 30, 31, 31, 0};
	vertCt = 32;
	indiceCt = 64;
	scale = 1;
	angle = 0;
	color = ringColor;
	speed = ringSpeed;
}

GammaRing::~GammaRing() {

}

void GammaRing::Update() {
	angle += speed * M_PI / 180;
	transformPoints(vectorData, worldData, vertCt, position, scale, angle);
}