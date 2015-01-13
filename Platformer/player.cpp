#include "player.h"

#define PLAYER_WIDTH  56
#define PLAYER_HEIGHT 74
Player::Player(TextureManager &textureManager, float x, float y)
	: Object(
			Object::Type::Obj,
			x, y, PLAYER_WIDTH, PLAYER_HEIGHT, // x, y, w, h
			0, 0,     // dx, dy
			true,     // solid
			0.00235,  // Gravity
			0.5       // Fall speed
	  ),
	  rectangle(sf::Vector2f((PLAYER_WIDTH / 2.0) - 8.0, PLAYER_HEIGHT / 2.0)),
	  texture("arthur1"),
	  moveSpeed(0.22), jumpSpeed(0.8)
{
	// Box
	rectangle.setOrigin(10.0f, 37.0f);
	rectangle.setScale(sf::Vector2f(2.0f, 2.0f));

	// Sprite
	sprite.setTexture(textureManager.getRef(texture));
	sprite.setOrigin(14.0f, 37.0f);
	sprite.setScale(sf::Vector2f(2.0f, 2.0f));

	// Animations
	animations["still"].push_back(sf::IntRect(7, 3, 24, 37));

	animations["crouch"].push_back(sf::IntRect(-2, 122, 33, 35));

	animations["jumpu"].push_back(sf::IntRect(0, 45, 38, 37));
	animations["jumpu2"].push_back(sf::IntRect(34, 45, 38, 37));

	animations["jumps"].push_back(sf::IntRect(78, 45, 36, 37));
	animations["jumps2"].push_back(sf::IntRect(120, 45, 36, 37));

	animations["jumpi"].push_back(sf::IntRect(152, 45, 28, 37));
	
	animations["run"].push_back(sf::IntRect(34, 3, 28, 37));
	animations["run"].push_back(sf::IntRect(151, 3, 28, 37));
	animations["run"].push_back(sf::IntRect(64, 3, 28, 37));
	animations["run"].push_back(sf::IntRect(92, 3, 28, 37));
	animations["run"].push_back(sf::IntRect(184, 3, 28, 37));
	animations["run"].push_back(sf::IntRect(118, 3, 28, 37));

	animations["throw"].push_back(sf::IntRect(3, 83, 28, 37));
	animations["throw"].push_back(sf::IntRect(33, 82, 36, 37));

	animations["throwi"].push_back(animations.at("throw")[0]);
	animations["throwi"].push_back(animations.at("throw")[1]);
	animations["throwi"].push_back(animations.at("jumpi")[0]);
	animations["throwi"].push_back(animations.at("jumpi")[0]);
	animations["throwi"].push_back(sf::IntRect(178, 45, 26, 37));
	animations["throwi"].push_back(sf::IntRect(178, 45, 26, 37));
	animations["throwi"].push_back(sf::IntRect(202, 45, 26, 37));
	animations["throwi"].push_back(sf::IntRect(202, 45, 26, 37));

	animations["throwc"].push_back(sf::IntRect(28, 122, 33, 35));
	animations["throwc"].push_back(sf::IntRect(59, 122, 38, 35));

	animation = "still";
	frame = 0;
}


/* Mutators */
void Player::setCrouching(bool c)
{
	crouching = c;

	if (getDY() != 0.0f || jumped) crouching = false;
}


/* Accessors */
int Player::getDir() const
{
	int dir = 1;

	if (sprite.getScale().x < 0.0f) dir = -1;

	return dir;
}

bool Player::placeFree(float x, float y, std::vector<Object*> const objects) const
{
	sf::FloatRect temp_rect(x - (sprite.getOrigin().x * sprite.getScale().x), y - (sprite.getOrigin().y * sprite.getScale().y), getWidth(), getHeight());

	return std::none_of(objects.begin(), objects.end(), [&](Object* const &elem)
	{
		return elem->isSolid() && temp_rect.intersects(elem->getRect());
	});
}


/* Actions */
void Player::draw(sf::RenderWindow &window)
{
	if (DEBUG_MODE)
	{
		rectangle.setScale(sprite.getScale());
		rectangle.setPosition(roundf(getX()), roundf(getY()));
	}
	
	int sign_scalex;
	if (sprite.getScale().x >= 0.0f) sign_scalex = 1;
	else sign_scalex = -1;

	if (jumped)
	{
		if (getDX() == 0.0f)
		{
			if (midThrow && jumps == 2) sprite.setPosition(roundf(getX()) - (6.0f * (float)sign_scalex), roundf(getY()));
			else if (midJump) sprite.setPosition(roundf(getX()) - (6.0f * (float)sign_scalex), roundf(getY()));
			else if (midThrow) sprite.setPosition(roundf(getX()), roundf(getY()));
			else sprite.setPosition(roundf(getX()) - (10.0f * (float)sign_scalex), roundf(getY()));
		}
		else if (midThrow) sprite.setPosition(roundf(getX()), roundf(getY()));
		else sprite.setPosition(roundf(getX()) - (5.0f * (float)sign_scalex), roundf(getY()));
	}
	else if (crouching) sprite.setPosition(roundf(getX()), roundf(getY()) + 12.0f);
	else sprite.setPosition(roundf(getX()), roundf(getY()));

	if (DEBUG_MODE) window.draw(rectangle);
	window.draw(sprite);
}

void Player::move(int dir)
{
	if (getDY() == 0 && !jumped) setDX(dir * moveSpeed);
	if (dir != 0) sprite.setScale(sf::Vector2f(2 * dir, 2));
}

void Player::jump(int dir, SoundManager &soundManager)
{
	if (jumps < 2)
	{
		if (jumps == 1)
		{
			setAnimation("jumpi");
			midJump = true;
			jumpTimer.restart();
		}

		setDY(-jumpSpeed);
		setDX(dir * moveSpeed);

		jumped = true;
		jumps++;

		soundManager.playSound("jump");
	}
}

void Player::throwWeapon(std::vector<Object*> &objects, int dir, TextureManager &textureManager, SoundManager &soundManager)
{
	if (!midThrow)
	{
		midThrow = true;
		throwTimer.restart();

		if (jumps == 0) throwTime = 0.14f;
		else if (jumps == 1 || flipped) throwTime = 0.13f;
		else
		{
			throwTime = 0.44f;
			flipped = true;
		}

		float adjx = -(float)getDir() * 20.0f, adjy = 0.0f;
		if (crouching)
		{
			adjx = -(float)getDir() * 8.0f;
			adjy = 24.0f;
		}

		Projectile* weapon = new Projectile(getX() + adjx, getY() - (35.0f * 2.0f) + adjy, dir, textureManager);
		objects.push_back(weapon);

		soundManager.playSound("throw");
	}
}

//double total_time = 0.0;

void Player::update(sf::Time deltaTime, sf::RenderWindow &window, sf::View &view, TextureManager &textureManager, SoundManager &soundManager, std::vector<Object*> const objects)
{
	double mstime = deltaTime.asMicroseconds() / 1000.0;
	//total_time += mstime;
	//printf("Total Time: %4.3f\n", (float)total_time);

	// Gravity
	if (placeFree(getX(), getY() + 1, objects)) setDY(getDY() + getGravity() * (float)mstime);
	else if (getDY() > 0.0f)
	{
		setDY(0);

		if (jumped)
		{
			jumped = false;
			flipped = false;
			jumps = 0;
		}

		soundManager.playSound("land");
	}
	else if (getDY() < 0 && !placeFree(getX(), getY() - 1, objects))
	{
		setDY(0); // Hitting head on the ceiling
		// TODO play sound?
	}

	//if (dy > maxFallSpeed) dy = maxFallSpeed;

	// Update Y
	for (float i = fabs(getDY()) * (float)mstime; i > 0; i--)
	{
		float j = copysign(i, getDY());
		if (placeFree(getX(), getY() + j, objects))
		{
			setY(getY() + j);
			break;
		}
	}

	// Update X
	if ((getDY() == 0.0f && !midThrow && !crouching) || jumped)
	{
		for (float i = fabs(getDX()) * (float)mstime; i > 0; i--)
		{
			float j = copysign(i, getDX());
			if (placeFree(getX() + j, getY(), objects))
			{
				setX(getX() + j);
				break;
			}
		}
	}

	if (getX() < sprite.getOrigin().x * 2.0f) setX(sprite.getOrigin().x * 2.0f);

	// Update view
	float vx = getX();
	float vy = getY();
	float vw = view.getSize().x;
	float vh = view.getSize().y;

	if (vx < vw / 2.0f) vx = vw / 2.0f;
	if (vy < vh / 2.0f) vy = vh / 2.0f;

	view.setCenter(vx, vh / 2.0f);
	window.setView(view);

	// Jump and Throw Timers
	if (midJump && jumpTimer.getElapsedTime().asSeconds() >= 0.2f) midJump = false;
	else if (midThrow && throwTimer.getElapsedTime().asSeconds() >= throwTime) midThrow = false;

	// Animation
	if (crouching)
	{
		if (midThrow) setAnimation("throwc");
		else setAnimation("crouch");
	}
	else if (midThrow)
	{
		if (jumps < 2) setAnimation("throw");
		else setAnimation("throwi");
	}
	else if (getDY() > 0.0f && !jumped) setAnimation("still"); // Falling
	else if (getDX() != 0.0f)
	{
		if (jumped)
		{
			if (jumps == 1) setAnimation("jumps");
			else setAnimation("jumps2");
		}
		else setAnimation("run");
	}
	else
	{
		if (jumped)
		{
			if (jumps == 1) setAnimation("jumpu");
			else setAnimation("jumpu2");
		}
		else setAnimation("still");
	}

	updateAnimation(deltaTime);
}

void Player::setAnimation(std::string name)
{
	if (!midJump && animation != name)
	{
		animation = name;
		frame = 0.0f;
	}
}

void Player::updateAnimation(sf::Time deltaTime)
{
	// Get current animation
	std::vector<sf::IntRect> &anim = animations.at(animation);
	int frames = anim.size();

	// Adjust frame
	if (frames > 1)
	{
		float speed = 60.0f / 5.2f;

		if (frame < (float)frames) frame += deltaTime.asSeconds() * speed;

		if (frame >= (float)frames) frame = 0.0f; // Reset animation
	}

	// Set TextureRect
	sprite.setTextureRect(anim[(int)frame]);
}

void Player::changeTexture(TextureManager &textureManager, std::string tex)
{
	texture = tex;
	sprite.setTexture(textureManager.getRef(texture));
}
