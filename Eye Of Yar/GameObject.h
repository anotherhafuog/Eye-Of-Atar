#ifndef GAMEOBJECT_H
#define GAMEOBJECT_H
#include <SDL.h>
#include <vector>
#include "drawFunctions.h"

class GameEntity {
	public:
		GameEntity() {
			vectorData = { {0,0} };
			position = { 0,0 };
			indices = { 0 };
			vertCt = 0;
			indiceCt = 0;
			scale = 0;
			angle = 0;
			color = { 0, 0, 0, 0 };
		}

		GameEntity(std::vector<SDL_Point> entData, SDL_Point entPos, std::vector<int> entInd, int entvCt, int entiCt, float entSc, float entAng, SDL_Color entColor) {
			vectorData = entData;
			position = entPos;
			vertCt = entvCt;
			indiceCt = entiCt;
			scale = entSc;
			angle = entAng;
			color = entColor;
		}

		~GameEntity() {

		}

		inline SDL_Point getPosition() {
			return position;
		}

		inline float getScale() {
			return scale;
		}

		inline float getAngle() {
			return angle;
		}

		inline SDL_Color getColor() {
			return color;
		}

		void Update() {
			transformPoints(vectorData, worldData, vertCt, position, scale, angle);
		}

		void Render() {
			drawVectorPic(worldData, indices, indiceCt, color);
		}

	protected:
		std::vector<SDL_Point> vectorData;
		std::vector<SDL_Point> worldData;
		SDL_Point position;
		std::vector<int> indices;
		int vertCt;
		int indiceCt;
		float scale;
		float angle;
		SDL_Color color;
};

#endif