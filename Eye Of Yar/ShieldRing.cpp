#include "ShieldRing.h"

namespace {
	constexpr float TWO_PI = 6.28318530718f;
	constexpr Uint8 BR_HIGH = 255;   // 50 pts
	constexpr Uint8 BR_MID = 160;   // 30 pts
	constexpr Uint8 BR_LOW = 80;    // 10 pts
}


ShieldRing::ShieldRing() {
	segs_.reserve(MAX_SEGMENTS);
	slots_.reserve(MAX_SEGMENTS);
	// Radii: inner ring's nearest hexagon tip (radius - 20 px) must stay
	// >= 96 px from the target, so inner radius = 116. Ring step 32 px and
	// counts = round(2*pi*r/32) keep neighbours touching (87 segments total).
	//             count radius angle speed  brightness
	rings_[0] = { 23,  116.f, 0.f,  1.2f,  BR_HIGH };   // inner, most points
	rings_[1] = { 29,  148.f, 0.f, -0.8f,  BR_MID };
	rings_[2] = { 35,  180.f, 0.f,  0.5f,  BR_LOW };   // outer

}

void ShieldRing::reset(int level) {
	segs_.clear();
	slots_.clear();
	if (level % 2 == 1) { layout_ = Layout::Rings;  buildRings(); }
	else { layout_ = Layout::Snakes; buildSnakes(); }
	alive_.assign(segs_.size(), 1);
}

void ShieldRing::Update(float dt) {
	if (layout_ == Layout::Rings) updateRings(dt);
	else                          updateSnakes(dt);
}

void ShieldRing::Render() {
	for (ShieldSeg& s : segs_) {
		if (!s.isAlive()) continue;
		// TODO: replace with however you draw a GameEntity
		// (member function or a function from drawFunctions.h)
		s.Render();
	}
}

int ShieldRing::hit(int i) {
	if (!alive_[i]) return 0;
	alive_[i] = 0;
	segs_[i].destroyed();
	return segs_[i].getPoints();
}

// ============================ Level 1: rings ================================

void ShieldRing::buildRings() {
	for (int r = 0; r < 3; ++r) {
		rings_[r].angle = 0.f;
		for (int i = 0; i < rings_[r].count; ++i) {
			segs_.emplace_back(rings_[r].brightness);
			slots_.push_back({ r, i });
		}
	}
	updateRings(0.f);
}

void ShieldRing::updateRings(float dt) {
	for (Ring& r : rings_) r.angle += r.angularSpeed * dt;

	for (size_t i = 0; i < segs_.size(); ++i) {
		const Ring& r = rings_[slots_[i].group];
		float a = r.angle + slots_[i].index * (TWO_PI / r.count);
		segs_[i].Update({(int)std::lround(1280 + r.radius * std::cos(a)),
			(int)std::lround(720 + r.radius * std::sin(a)) });

	}
}

// ===================== Level 2: serpentine snake square =====================
//
// One path visits every cell: row 0 left->right, drop one row, row 1
// right->left, drop, ... With an odd gridN the last row runs left->right and
// ends bottom-right. Snakes are consecutive runs of segments on that path, end
// to end, so the block is solid at start. Every segment advances along the
// path; one passing the last cell wraps (teleports) to the first, top-left.

void ShieldRing::pathCell(int k, float& x, float& y) const {
	const int N = snk_.gridN;
	const int row = k / N;
	const int c0 = k % N;
	const int col = (row % 2 == 0) ? c0 : N - 1 - c0;

	const float spanW = (N - 1) * snk_.cellW + snk_.rowShift;
	x = 1280 - spanW * 0.5f + col * snk_.cellW + ((row % 2) ? snk_.rowShift : 0.f);
	y = 720 + (row - (N - 1) * 0.5f) * snk_.rowH;
}

void ShieldRing::buildSnakes() {
	const int cap = MAX_SEGMENTS;
	const int total = std::min(snk_.gridN * snk_.gridN, cap);
	snk_.progress = 0.f;

	for (int i = 0; i < total; ++i) {
		int p = i % snk_.snakeLen;                 // position inside its snake
		Uint8 b = (p == 0) ? BR_HIGH
			: (p < snk_.snakeLen / 2.f ? BR_MID : BR_LOW);
		segs_.emplace_back(b);
		slots_.push_back({ 0, i });
	}
	updateSnakes(0.f);
}

void ShieldRing::updateSnakes(float dt) {
	const int total = snk_.gridN * snk_.gridN;     // path length in cells
	snk_.progress = std::fmod(snk_.progress + snk_.speed * dt, (float)total);

	const float base = snk_.smooth ? snk_.progress : std::floor(snk_.progress);

	for (size_t i = 0; i < segs_.size(); ++i) {
		float q = std::fmod(base - slots_[i].index, (float)total);
		if (q < 0.f) q += total;                   // wrap = teleport to top-left

		int   k = std::min(total - 1, (int)q);
		float f = q - k;

		float x, y;
		pathCell(k, x, y);
		if (k < total - 1) {                       // last cell: hold, then wrap
			float nx, ny;
			pathCell(k + 1, nx, ny);
			x += (nx - x) * f;
			y += (ny - y) * f;
		}
		segs_[i].Update({(int)std::lround(x),(int)std::lround(y)});
	}
}


