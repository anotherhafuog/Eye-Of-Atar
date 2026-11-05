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

	//setting defaults
	DvectorData = vectorData;
	Dposition = position;
	Dindices = indices;
	DvertCt = vertCt;
	DindiceCt = indiceCt;
	Dscale = scale;
	Dangle = angle;
	Dcolor = color;

	hitbox.x = position.x - (scale/2);
	hitbox.y = position.y - (scale/2);
	hitbox.w = scale;
	hitbox.h = scale;

	state = EyeState::IDLE;
	velocity = { 0,0 };
	isActive = 0;
	audio::init();
	zoom = audio::loadSfx("Sounds/TRANSPARENT WALLS.mp3");
	srand(time(NULL));// for rand seeding
}

Eyeball::~Eyeball() {
	Mix_FreeChunk(zoom);
	audio::shutdown();
}

void Eyeball::Update(int frameCount, int gameSpeed, SDL_Point atarPos, const bool pressed[SDL_NUM_SCANCODES]){
	if (state == EyeState::IDLE) {
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
	}
	else if (state == EyeState::BLINKING) {

	}

	else if (state == EyeState::BOMB) {
		position.x = atarPos.x + (rand() % 9);
		position.y = atarPos.y + (rand() % 9);
		float dx = atarPos.x - 1280;
		float dy = atarPos.y - 720;
		float d2 = dx * dx + dy * dy;
		if(pressed[SDL_SCANCODE_SPACE] && (d2 >= 675 * 675)) {
			isActive = 1;
			state = EyeState::TRAVELING;
			audio::playSfx(zoom);
		}
	}

	else if (state == EyeState::TRAVELING) {
		if (isActive) {
			position = atarPos;
			float dx = 1280 - atarPos.x;
			float dy = 720 - atarPos.y;
			float len = std::sqrt(dx * dx + dy * dy);

			if (len > 0.0001f) {          // avoid divide by zero
				velocity.x = (dx / len) * 8;  // speed in pixels/second
				velocity.y = (dy / len) * 8;
			}
			else {
				velocity.x = velocity.y = 0;
			}
			isActive = 0;
		}
		position.x += velocity.x;
		position.y += velocity.y;
	}

	else if (state == EyeState::DANGEROUS) {
		if (frameCount % 2 == 0) {
			color = { 255,255,255,255 };
		}
		else {
			color = { 255,0,0,255 };
		}
		position.x -= velocity.x;
		position.y -= velocity.y;
		if (position.x + scale < 320 || position.x - scale > 2240 ||
			position.y + scale < 0 || position.y - scale > 1440) {
			reset();
		}
	}

	hitbox.x = position.x - (scale / 2);
	hitbox.y = position.y - (scale / 2);
	transformPoints(vectorData, worldData, vertCt, position, scale, angle);
}

void Eyeball::changeState(int newState) {
	state = static_cast<EyeState>(newState);
}

void Eyeball::reset() {
	//setting defaults
	vectorData = DvectorData;
	position = Dposition;
	indices = Dindices;
	vertCt = DvertCt;
	indiceCt = DindiceCt;
	scale = Dscale;
	angle = Dangle;
	color = Dcolor;


	hitbox.x = position.x - (scale / 2);
	hitbox.y = position.y - (scale / 2);
	state = EyeState::IDLE;
}

const SDL_Rect* Eyeball::getHitbox() {
	return &hitbox;
}

int Eyeball::getState() {
	return static_cast<int>(state);
}