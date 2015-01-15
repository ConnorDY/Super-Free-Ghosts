#include "object.h"

Object::Object(Type type, float x, float y, float width, float height, float dx, float dy, bool solid, float gravity, float maxFallSpeed)
	: x(x), y(y),
	  dx(dx), dy(dy),
	  width(width), height(height),
	  gravity(gravity),
	  maxFallSpeed(maxFallSpeed),
	  solid(solid),
	  del(false),
	  _type(type)
{
}

Object::~Object()
{

}


/* Mutators */
void Object::setX(float xx) { x = xx; }
void Object::setY(float yy) { y = yy; }
void Object::setDX(float dxx) { dx = dxx; }
void Object::setDY(float dyy) { dy = dyy; }
void Object::setWidth(float w) { width = w; }
void Object::setHeight(float h) { height = h; }
void Object::setGravity(float g) { gravity = g; }
void Object::setSolid(bool s) { solid = s; }
void Object::setDelete(bool d) { del = d; }
void Object::setType(Object::Type t) { _type = t; }


/* Accessors */
float Object::getX() const { return x; }
float Object::getY() const { return y; }
float Object::getDX() const { return dx; }
float Object::getDY() const { return dy; }
float Object::getWidth() const { return width; }
float Object::getHeight() const { return height; }
float Object::getGravity() const { return gravity; }
bool Object::isSolid() const { return solid; }
bool Object::shouldDelete() const { return del; }
Object::Type Object::getType() const { return _type; }

sf::FloatRect Object::getRect() const
{
	return sf::FloatRect(x, y, width, height);
}

bool Object::placeFree(float xx, float yy, std::vector<Object*> const objects) const
{
	float adj = 0.0f;

	if (_type == Type::Projectile && dx < 0.0f) adj = -width;
	sf::FloatRect temp_rect(xx + adj, yy, width, height);

	return std::none_of(objects.begin(), objects.end(), [&](Object* const &elem)
	{
		return this != elem && elem->isSolid() && temp_rect.intersects(elem->getRect());
	});
}


/* Actions */
void Object::draw(sf::RenderWindow &window)
{
	
}

void Object::update(sf::Time deltaTime, std::vector<Object*> const objects)
{
	double mstime = deltaTime.asMicroseconds() / 1000.0;

	// Gravity
	if (placeFree(x, y + 1, objects)) dy += gravity * (float)mstime;
	else if (dy > 0.0f) dy = 0.0f;

	//if (dy > maxFallSpeed) dy = maxFallSpeed;

	// Update Y
	for (float i = fabs(dy) * (float)mstime; i > 0; i--)
	{
		float j = copysign(i, dy);
		if (placeFree(x, y + j, objects))
		{
			y += j;
			break;
		}
	}

	// Update X
	for (float i = fabs(dx) * (float)mstime; i > 0; i--)
	{
		float j = copysign(i, dx);
		if (placeFree(x + j, y, objects))
		{
			x += j;
			break;
		}
	}
}
