#ifndef SHIELDRING_H
#define SHIELDRING_H
#include <SDL.h>
#include <vector>
#include <algorithm>
#include <cmath>
#include "drawFunctions.h"
#include "GameObject.h"
#include "ShieldSeg.h"

class ShieldRing{
	public:
		enum class Layout { Rings, Snakes };

		// ---- Level 1: concentric rotating rings -------------------------------
		struct Ring {
			int   count;          // segments in this ring
			float radius;         // px from the target
			float angle;          // current rotation offset (rad)
			float angularSpeed;   // rad/s, sign = direction
			Uint8 brightness;     // 255 = 50 pts, 160 = 30 pts, 80 = 10 pts
		};

		// ---- Level 2: solid hex-packed square, snakes end to end --------------
		// Defaults tile your hexagon exactly: width 16*scale = 32 px,
		// row spacing 15*scale = 30 px, odd rows shifted half a cell (16 px).
		// For a plain square grid use rowShift = 0 and cellW = rowH = segment size.
		struct SnakeLayout {
			int   gridN = 7;       // ODD, so the path ends bottom-right
			float cellW = 32.f;
			float rowH = 30.f;
			float rowShift = 16.f;
			int   snakeLen = 7;       // segments per snake (last snake may be shorter)
			float speed = 4.f;     // cells per second
			bool  smooth = true;    // false = hop one whole cell at a time
			float progress = 0.f;     // head position along the path, in cells
		};

		static constexpr int MAX_SEGMENTS = 90;   // level 1 uses 87, level 2 uses 49

		ShieldRing();

		void reset(int);      // 1 = rings, anything else = snakes
		void Update(float dt);
		void Render();

		Ring& ring(int i) { return rings_[i]; }
		SnakeLayout& snakes() { return snk_; }

		// Collision access. Dead segments have a 0x0 hitbox, so
		// SDL_HasIntersection ignores them without an isAlive check.
		int  size() const { return (int)segs_.size(); }
		bool alive(int i) const { return alive_[i] != 0; }
		const SDL_Rect* hitbox(int i) { return segs_[i].getHitbox(); }
		int  hit(int i);

	private:
		struct Slot { int group; int index; };

		void buildRings();
		void updateRings(float dt);

		void buildSnakes();
		void updateSnakes(float dt);
		void pathCell(int k, float& x, float& y) const;

		float  cx_, cy_;
		Layout layout_ = Layout::Rings;

		std::vector<ShieldSeg> segs_;     // reserved to MAX_SEGMENTS, never reallocates
		std::vector<Slot>      slots_;
		std::vector<char> alive_;             // 1 = alive, 0 = destroyed
		Ring                   rings_[3];
		SnakeLayout            snk_;

};

#endif
