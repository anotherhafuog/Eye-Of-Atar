#include "GammaField.h"
using namespace std;

GammaField::GammaField(int gameSpeed) 
: ring0(gameSpeed, 500, SDL_Color{ 255, 0, 255, 64 }),
ring1(-gameSpeed, 550, SDL_Color{ 0, 255, 255, 64 }),
ring2(gameSpeed, 600, SDL_Color{ 0, 255, 0, 64 }),
ring3(-gameSpeed, 650, SDL_Color{ 255, 255, 0, 64 }) 
{
}

GammaField::~GammaField() {

}

void GammaField::Update() {
	ring0.Update();
	ring1.Update();
	ring2.Update();
	ring3.Update();
}

void GammaField::Render() {
	ring0.Render();
	ring1.Render();
	ring2.Render();
	ring3.Render();
}