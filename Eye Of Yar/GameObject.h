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

			//setting defaults
			DvectorData = vectorData;
			Dposition = position;
			Dindices = indices;
			DvertCt = vertCt;
			DindiceCt = indiceCt;
			Dscale = scale;
			Dangle = angle;
			Dcolor = color;
		}

		GameEntity(std::vector<SDL_Point> entData, SDL_Point entPos, std::vector<int> entInd, int entvCt, int entiCt, float entSc, float entAng, SDL_Color entColor) {
			vectorData = entData;
			position = entPos;
			vertCt = entvCt;
			indiceCt = entiCt;
			scale = entSc;
			angle = entAng;
			color = entColor;

			//setting defaults
		}

		~GameEntity() {

		}

		void reset() {
			vectorData = DvectorData;
			position = Dposition;
			indices = Dindices;
			vertCt = DvertCt;
			indiceCt = DindiceCt;
			scale = Dscale;
			angle = Dangle;
			color = Dcolor;
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

		//defaults for resetting
		std::vector<SDL_Point> DvectorData;
		SDL_Point Dposition;
		std::vector<int> Dindices;
		int DvertCt;
		int DindiceCt;
		float Dscale;
		float Dangle;
		SDL_Color Dcolor;
};

#endif