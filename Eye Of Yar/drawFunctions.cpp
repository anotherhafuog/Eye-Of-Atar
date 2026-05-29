#include "drawFunctions.h"
using namespace std;

SDL_Window* window = nullptr;
SDL_Renderer* renderer = nullptr;

float thickness;



void initWindow(int windowSizeX, int windowSizeY, Uint32 flags, float userThickness) {
	SDL_Init(SDL_INIT_VIDEO);
	SDL_CreateWindowAndRenderer(windowSizeX, windowSizeY, flags, &window, &renderer);
	SDL_RenderSetScale(renderer, 1, 1); //initialize window
	SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_ADD); // sets colors to additive blend
	thickness = userThickness;
}

void drawVec(const SDL_Point pt0, const SDL_Point pt1, SDL_Color color) {
	float x0 = pt0.x;
	float x1 = pt1.x;

	float y0 = pt0.y;
	float y1 = pt1.y;// define points from struct

	float dx = x1 - x0;
	float dy = y1 - y0;
	float length = sqrt((dx * dx) + (dy * dy)); //find length of vector

	float ptx = (-dy / length) * (thickness / 2.0f);
	float pty = (dx / length) * (thickness / 2.0f); //calculate offsets from origin points for rect

	struct SDL_Vertex verts[4];
	verts[0].position = { x0 + ptx, y0 + pty };
	verts[1].position = { x1 + ptx, y1 + pty };
	verts[2].position = { x1 - ptx, y1 - pty };
	verts[3].position = { x0 - ptx, y0 - pty }; // Compute points for rect

	for (int i = 0; i < 4; ++i) {
		verts[i].color = color;
	} //set color

	const int indices[6] = { 0, 1, 2, 2, 3, 0 };

	SDL_RenderGeometry(renderer, nullptr, verts, 4, indices, 6); //draw
}

void drawVectorPic(const vector<SDL_Point> &verts, const vector<int> &indices, int indiceCt, SDL_Color color) {
	for (int i = 0; i < indiceCt - 1; i += 2) {
		drawVec(verts[indices[i]], verts[indices[i + 1]], color);
	} //read two indices, get the points from verts and draw a line between
}

void transformPoints(const vector<SDL_Point> &localPts, vector<SDL_Point> &worldPts, int ptCt, SDL_Point center, float scale, float angle) {
	worldPts.resize(ptCt);
	float cs = cosf(angle);
	float sn = sinf(angle);

	for (int i = 0; i < ptCt; i++) {
		float x = localPts[i].x * scale;
		float y = localPts[i].y * scale;

		worldPts[i].x = (int)(x * cs - y * sn + center.x);
		worldPts[i].y = (int)(x * sn + y * cs + center.y);
	}
}

void printChar(unsigned char theChar, SDL_Point centerPt, SDL_Color color, float scale) {
	vector<SDL_Point> worldPts;
	const int charArrSize = 36;
	int letterIndex = -1;

	if (theChar >= 48 && theChar <= 57) {
		letterIndex = theChar - 48;       // digits 0-9
	}
	else if (theChar >= 65 && theChar <= 90) {
		letterIndex = theChar - 65 + 10;  // letters, A=10, B=11, etc.
	}

	if (letterIndex >= 0 && letterIndex < charArrSize) {

		vector<SDL_Point> charVecs[charArrSize] = {
			{{0,0}, {8,0}, {8,12}, {0,12}}, //0
			{{4,0}, {4,12}}, //1
			{{0,0}, {8,0}, {8,6}, {0,6}, {0,12}, {8,12}}, //2
			{{0,0}, {8,0}, {8,6}, {0,6}, {8,12}, {0,12}}, //3
			{{0,0}, {0,6}, {8,6}, {8,0}, {8,12}}, //4
			{{8,0}, {0,0}, {0,6}, {8,6}, {8,12}, {0,12}}, //5
			{{0,0}, {0,12}, {8,12}, {8,6}, {0,6}}, //6
			{{0,0}, {8,0}, {8,12}}, //7
			{{0,0}, {8,0}, {8,12}, {0,12}, {0,6}, {8,6}}, //8
			{{8,0}, {0,0}, {0,6}, {8,6}, {8,12}}, //9
			{{0,12}, {0,4}, {4,0}, {8,4}, {8,12}, {0,6}, {8,6}}, //A
			{{0,0}, {6,0}, {8,2}, {8,4}, {6,6}, {0,6}, {8,8}, {8,10}, {6,12}, {0,12}}, //B
			{{8,0}, {0,0}, {0,12}, {8,12}}, // C
			{{0,0}, {4,0}, {8,4}, {8,8}, {4,12}, {0,12}}, //D
			{{8,0}, {0,0}, {0,12}, {8,12}, {0,6}, {6,6}}, //E
			{{8,0}, {0,0}, {0,12}, {0,6}, {6,6}}, //F
			{{8,4}, {8,0}, {0,0}, {0,12}, {8,12}, {8,8}, {4,8}}, //G
			{{0,0}, {0,12}, {8,0}, {8,12}, {0,6}, {8,6}}, //H
			{{0,0}, {8,0}, {8,12}, {0,12}, {4,0}, {4,12}}, //I
			{{8,0}, {8,12}, {4,12}, {0,8}}, //J
			{{0,0}, {0,12}, {6,0}, {0,6}, {6,12}}, //K
			{{0,0}, {0,12}, {8,12}}, //L
			{{0,12}, {0,0}, {4,4}, {8,0}, {8,12}}, //M
			{{0,12}, {0,0}, {8,12}, {8,0}}, //N
			{{0,0}, {8,0}, {8,12}, {0,12}}, //O
			{{0,12}, {0,0}, {8,0}, {8,6}, {0,6}}, //P
			{{0,0}, {8,0}, {8,8}, {8,8}, {4,12}, {0,12}, {4,8}, {8,12}}, //Q
			{{0,12}, {0,0}, {8,0}, {8,6}, {0,6}, {8,12}}, //R
			{{8,0}, {0,0}, {0,6}, {8,6}, {8,12}, {0,12}}, //S
			{{0,0}, {8,0}, {4,0}, {4,12}}, //T
			{{0,0}, {8,0}, {8,12}, {0,12}}, //U
			{{0,0}, {4,12}, {8,0}}, //V
			{{0,0}, {0,12}, {4,8}, {8,12}, {8,0}}, //W
			{{0,0}, {8,0}, {8,12}, {0,12}}, //X
			{{0,0}, {4,4}, {8,0}, {4,12}}, //Y
			{{0,0}, {8,0}, {8,12}, {0,12}} //Z

		};

		vector <int> charIndices[charArrSize] = {
			{0,1,1,2,2,3,3,0}, //0
			{0,1}, //1
			{0,1,1,2,2,3,3,4,4,5}, //2
			{0,1,1,2,2,3,2,4,4,5}, //3
			{0,1,1,2,3,4},  //4
			{0,1,1,2,2,3,3,4,4,5}, //5
			{0,1,1,2,2,3,3,4}, //6
			{0,1,1,2}, //7
			{0,1,1,2,2,3,3,0,4,5}, //8
			{0,1,1,2,2,3,0,4}, //9
			{0,1,1,2,2,3,3,4,5,6}, //A
			{0,1,1,2,2,3,3,4,4,5,4,6,6,7,7,8,8,9,9,0}, //B
			{0,1,1,2,2,3}, //C
			{0,1,1,2,2,3,3,4,4,5,5,0}, //D
			{0,1,1,2,2,3,4,5}, //E
			{0,1,1,2,3,4}, //F
			{0,1,1,2,2,3,3,4,4,5,5,6}, //G
			{0,1,2,3,4,5}, //H
			{0,1,2,3,4,5}, //I
			{0,1,1,2,2,3}, //J
			{0,1,2,3,3,4}, //K
			{0,1,1,2}, //L
			{0,1,1,2,2,3,3,4}, //M
			{0,1,1,2,2,3}, //N
			{0,1,1,2,2,3,3,0}, //O
			{0,1,1,2,2,3,3,4}, //P
			{0,1,1,2,2,3,3,4,4,5,5,0,6,7}, //Q
			{0,1,1,2,2,3,3,4,4,5}, //P
			{0,1,1,2,2,3,3,4,4,5}, //S
			{0,1,2,3}, //T
			{0,3,3,2,2,1}, //U
			{0,1,1,2}, //V
			{0,1,1,2,2,3,3,4}, //W
			{0,2,1,3}, //X
			{0,1,1,2,1,3}, //Y
			{0,1,1,3,3,2} //Z
		};

		int charPtCt[charArrSize] = {
			4, //0
			2, //1
			6, //2
			6, //3
			5, //4
			6, //5
			5, //6
			3, //7
			6, //8
			5, //9
			7, //A
			10, //B
			4, //C
			6, //D
			6, //E
			5, //F
			7, //G
			6, //H
			6, //I
			4, //J
			5, //K
			3, //L
			5, //M
			4, //N
			4, //O
			5, //P
			8, //Q
			6, //R
			6, //S
			4, //T
			4, //U
			3, //V
			5, //W
			4, //X
			4, //Y
			4 //Z
		};

		int charIndiceCt[charArrSize] = {
			8, //0
			2, //1
			10, //2
			10, //3
			6, //4
			10, //5
			8, //6
			4, //7
			10, //8
			8, //9
			10, //A
			20, //B
			6, //C
			12, //D
			8, //E
			6, //F
			12, //G
			6, //H
			6, //I
			6, //J
			6, //K
			4, //L
			8, //M
			6, //N
			8, //O
			8, //P
			14, //Q
			10, //R
			10, //S
			4, //T
			6, //U
			4, //V
			8, //W
			4, //X
			6, //Y
			6, //Z
		};

		transformPoints(charVecs[letterIndex], worldPts, charPtCt[letterIndex], centerPt, scale, 0);
		drawVectorPic(worldPts, charIndices[letterIndex], charIndiceCt[letterIndex], color);
	}
	else if (theChar == ' ') {
		//do nothing
	}
	else {
		transformPoints({ {0,0}, {8,0}, {8,6}, {0,6}, {0,8}, {0,10}, {0,12} },
			worldPts, 7, centerPt, scale, 0);
		drawVectorPic(worldPts, { 0,1,1,2,2,3,3,4,5,6 }, 10, color);
	}// draw ?
}

void printString(string theString, SDL_Point pt, SDL_Color color, float scale) {
	for (char& c : theString) {
		printChar(c, pt, color, scale);
		pt.x += 10 * scale;
	}
}

void renderFrame() {
	SDL_RenderPresent(renderer);
}

void clearScreen() {
	SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND); // sets colors to blend
	SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
	SDL_RenderClear(renderer); // screen wipe
	SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_ADD); // resets to additive blend
}

void killRender() {
	SDL_DestroyWindow(window);
	SDL_DestroyRenderer(renderer);
	SDL_Quit();//deallocate and close
}