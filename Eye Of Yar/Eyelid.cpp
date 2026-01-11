#include "Eyelid.h"
using namespace std;

Eyelid::Eyelid() {
	vectorData = { {-2,2}, {2,2}, {5,0}, {3,-2}, {-3,-2}, {-5,0} };
	position = { 1280,720 };
	indices = { 0, 1, 1, 2, 2, 3, 3, 4, 4, 5, 5, 0 }; //important to have 6 for eye closing animation
	vertCt = 6;
	indiceCt = 12;
	scale = 16;
	angle = 0;
	color = { 255, 0, 0, 128 };
}

Eyelid::~Eyelid() {

}