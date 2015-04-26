#include "trident.h"
#include "sprite_util.h"
#include "room.h"

namespace
{
	sf::IntRect const BBOX(27, 10, 35, 8);
	int const SPRITE_WIDTH = 67, SPRITE_HEIGHT = 29;
	int const TRAIL_LENGTH = 40;
}

Trident::Trident(Room &room, float x, float y, int dir)
	: Trident(room, x, y, BBOX.width, BBOX.height, dir)
{
	animationFrames.emplace_back(0, 0, SPRITE_WIDTH, SPRITE_HEIGHT);
	SpriteUtil::setOriginAndScale(sprite, sf::Vector2f(static_cast<float>(dir), 1.0f), BBOX);
}

Trident::Trident(Room &room, float x, float y, int width, int height, int dir)
	: Weapon(room, x, y, width, height, 0.3f * dir, 0, 0, 0, 5),
	  trailX(x)
{
	destroyedOnHit = false;
	setDepth(-4);
	sprite.setTexture(room.textureManager.getRef("trident"));
}

Trident::~Trident()
{
}

void Trident::draw(sf::RenderWindow &window)
{
	auto rect = getRect();
	/*float trailAdj;
	// Update trails
	if (x < trailX)
	{
		trailX = std::min<float>(x + 40, trailX);
		trailAdj = rect.width;
	}
	else
	{
		trailX = std::max<float>(x - 40, trailX);
		trailAdj = 0;
	}

	auto trailPoints = std::minmax<float>(x, trailX);

	// Draw trails
	auto trailTop = rect.top + rect.height / 2 - 2;
	sf::RectangleShape trail(sf::Vector2f(trailPoints.second - trailPoints.first, 3));
	trail.setPosition(trailPoints.first + trailAdj, trailTop);
	trail.setFillColor(sf::Color(255, 255, 255, 64));
	window.draw(trail);

	trail.setPosition(trailPoints.first + trailAdj, trailTop + 1);
	trail.setSize(sf::Vector2f(trailPoints.second - trailPoints.first, 1));
	trail.setFillColor(sf::Color(255, 255, 255, 128));
	window.draw(trail);*/
	// Draw sprite
	Weapon::draw(window);
}

Trident* Trident::spawnAdjusted(Room &room, float x, float y, int dir)
{
	if (dir < 0) x -= BBOX.width;
	return new Trident(room, x, y, dir);
}

bool Trident::canThrow(Room const &room)
{
	auto &objs = room.getObjects();
	return std::find_if(
			objs.begin(), objs.end(),
			[](Object *obj) { return dynamic_cast<Trident*>(obj) != nullptr; }
		) == objs.end();
}
