#include "object.h"
#include "globals.h"
#include "room.h"

Object::Object(Room &room, float x, float y, float width, float height, float dx, float dy, bool solid, float gravity, float maxFallSpeed)
	: room(room), x(x), y(y),
	  dx(dx), dy(dy),
	  width(width), height(height),
	  gravity(gravity),
	  maxFallSpeed(maxFallSpeed),
	  depth(0),
	  solid(solid),
	  del(false)
{
}

Object::~Object()
{

}


/* Accessors */
int Object::getDepth() const { return depth; }
bool Object::isSolid() const { return solid; }
bool Object::shouldDelete() const { return del; }

sf::FloatRect Object::getRect() const
{
	return sf::FloatRect(x, y, width, height);
}

bool Object::placeFree(float xx, float yy) const
{
	sf::FloatRect temp_rect(xx, yy, width, height);

	if (room.heightmapIntersects(temp_rect)) return false;
	if (room.exceedsHorizontalBounds(temp_rect)) return false;

	auto const &objects = room.getObjects();
	return std::none_of(objects.begin(), objects.end(), [&](Object* const &elem)
	{
		return this != elem && elem->isSolid() && temp_rect.intersects(elem->getRect()) && this->canCollideWith(elem) && elem->canCollideWith(this);
	});
}

std::vector<Object*> Object::allCollisions(float xx, float yy) const
{
	std::vector<Object*> result;
	auto myRect = getRect();
	sf::FloatRect temp_rect(xx + myRect.left - x, yy + myRect.top - y, myRect.width, myRect.height);

	auto const &objects = room.getObjects();
	std::copy_if(objects.begin(), objects.end(), std::back_inserter(result), [&](Object* const &elem)
	{
		return this != elem && temp_rect.intersects(elem->getRect());
	});
	return result;
}

bool Object::canCollideWith(Object const*) const
{
	return true;
}


/* Mutators*/
void Object::setX(float xx)
{
	x = xx;
}

void Object::setY(float yy)
{
	y = yy;
}

void Object::setDepth(int d)
{
	depth = d;
}


/* Actions */
void Object::update(sf::Time deltaTime)
{
	applyGravityOnce(deltaTime);
	applyVelocityOnce(deltaTime, dx, dy);
	pushOutOfHeightmap();
}

void Object::applyGravityOnce(sf::Time deltaTime)
{
	if (gravity == 0) return;

	float mstime = deltaTime.asMicroseconds() / 1000.0f;

	if (placeFree(x, y + 1)) dy += gravity * mstime;
	else if (dy > 0.0f) dy = 0.0f;

	if (dy > maxFallSpeed) dy = maxFallSpeed;
}

void Object::applyVelocityOnce(sf::Time deltaTime, float velX, float velY)
{
	float mstime = deltaTime.asMicroseconds() / 1000.0f;

	// Update Y
	for (float i = fabs(velY) * mstime; i > 0; i--)
	{
		float j = copysign(i, velY);
		if (placeFree(x, y + j))
		{
			y += j;
			break;
		}
	}

	bool shouldSlopeWalk = velY == 0 && !placeFree(x, y + 1);
	// Update X
	for (float i = fabs(velX) * mstime; i > 0; i--)
	{
		float xAdjust = copysign(i, velX);

		// We search for a y-adjustment, these are our bounds
		float yAdjustStart = 0;
		float yAdjustEnd = 0;

		if (shouldSlopeWalk)
		{
			yAdjustStart = round(maxFallSpeed * mstime);
			yAdjustEnd = -4;
		}

		// Find an appropriate y-adjustment for slope walking
		for (float yAdjust = yAdjustStart; yAdjust >= yAdjustEnd; yAdjust--)
		{
			if (placeFree(x + xAdjust, y + yAdjust))
			{
				x += xAdjust;
				y += yAdjust;

				return;
			}
		}
	}
}

void Object::pushOutOfHeightmap()
{
	if (room.heightmapIntersects(sf::FloatRect(x, y, width, height)))
		y = room.getMinTerrainYBetween(x, x + width) - height;
}

void Object::pushOutOfSolids()
{
	pushOutOfHeightmap();

	// Move out of any solid objects
	for (auto obj : allCollisions(x, y))
		if (obj->isSolid())
		{
			auto objbbox = obj->getRect();
			y = std::min<int>(y, objbbox.top - height);
		}
}

void Object::kill()
{
	if (del) return;
	onDeath();
	del = true;
}

void Object::onDeath()
{
}

void Object::onDoubleJumpedOver()
{
}
