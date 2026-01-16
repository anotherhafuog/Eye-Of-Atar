#ifndef GAMMAFIELD_H
#define GAMMAFIELD_H
#include <SDL.h>
#include <vector>
#include <cmath>
#include "drawFunctions.h"
#include "GameObject.h"
#include "GammaRing.h"

class GammaField : public GameEntity {
private:
	GammaRing ring0;
	GammaRing ring1;
	GammaRing ring2;
	GammaRing ring3;
public:
	GammaField(int);

	~GammaField();

	void Update();

	void Render();
};

#endif