#include "player.h"

Player::Player(TextureManager &textureManager, float xx, float yy)
{
	// Settings
	jumped = false;
	midJump = false;
	midThrow = false;
	flipped = false;
	crouching = false;
	jumps = 0;
	throwTime = 0;

	width = 56.0f;
	height = 74.0f;

	float adj = 10.0f;
	moveSpeed = 2.2f / adj;
	jumpSpeed = 8.00f / adj;
	gravity = 0.235f / (adj * adj);
	maxFallSpeed = 5.0f / adj;

	x = xx;
	y = yy;

	dx = 0.0f;
	dy = 0.0f;

	// Box
	rectangle.setSize(sf::Vector2f((width / 2.0f) - 8.0f, height / 2.0f));
	rectangle.setOrigin(10.0f, 37.0f);
	rectangle.setScale(sf::Vector2f(2.0f, 2.0f));

	// Sprite
	texture = "arthur1";
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
void Player::setX(float xx) { x = xx; }
void Player::setY(float yy) { y = yy; }
void Player::setDX(float dxx) { dx = dxx; }
void Player::setDY(float dyy) { dy = dyy; }
void Player::setCrouching(bool c)
{
	crouching = c;

	if (dy != 0.0f || jumped) crouching = false;
}


/* Accessors */
float Player::getX() { return x; }
float Player::getY() { return y; }
float Player::getDX() { return dx; }
float Player::getDY() { return dy; }
int Player::getDir()
{
	int dir = 1;

	if (sprite.getScale().x < 0.0f) dir = -1;

	return dir;
}

sf::FloatRect Player::getRect()
{
	return sf::FloatRect(x, y, width, height);
}

bool Player::placeFree(float xx, float yy, std::vector<Object*> const objects) const
{
	sf::FloatRect temp_rect(xx - (sprite.getOrigin().x * sprite.getScale().y), yy - (sprite.getOrigin().y * sprite.getScale().y), width, height);

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
		rectangle.setPosition(roundf(x), roundf(y));
	}
	
	int sign_scalex;
	if (sprite.getScale().x >= 0.0f) sign_scalex = 1;
	else sign_scalex = -1;

	if (jumped)
	{
		if (dx == 0.0f)
		{
			if (midThrow && jumps == 2) sprite.setPosition(roundf(x) - (6.0f * (float)sign_scalex), roundf(y));
			else if (midJump) sprite.setPosition(roundf(x) - (6.0f * (float)sign_scalex), roundf(y));
			else if (midThrow) sprite.setPosition(roundf(x), roundf(y));
			else sprite.setPosition(roundf(x) - (10.0f * (float)sign_scalex), roundf(y));
		}
		else if (midThrow) sprite.setPosition(roundf(x), roundf(y));			
		else sprite.setPosition(roundf(x) - (5.0f * (float)sign_scalex), roundf(y));
	}
	else if (crouching) sprite.setPosition(roundf(x), roundf(y) + 12.0f);
	else sprite.setPosition(roundf(x), roundf(y));

	if (DEBUG_MODE) window.draw(rectangle);
	window.draw(sprite);
}

void Player::move(int dir)
{
	if (dy == 0.0f && !jumped) setDX((float)dir * moveSpeed);
	if (dir != 0) sprite.setScale(sf::Vector2f(2.0f * (float)dir, 2.0f));
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

		Projectile* weapon = new Projectile(x + adjx, y - (35.0f * 2.0f) + adjy, dir, textureManager);
		objects.push_back(weapon);

		soundManager.playSound("throw");
	}
}

//double total_time = 0.0;

void Player::update(sf::Time deltaTime, sf::RenderWindow &window, sf::View &view, TextureManager &textureManager, SoundManager &soundManager, std::vector<Object*> objects)
{
	double mstime = deltaTime.asMicroseconds() / 1000.0;
	//total_time += mstime;
	//printf("Total Time: %4.3f\n", (float)total_time);

	// Gravity
	if (placeFree(x, y + 1, objects)) dy += gravity * (float)mstime;
	else if (dy > 0.0f)
	{
		dy = 0.0f;

		if (jumped)
		{
			jumped = false;
			flipped = false;
			jumps = 0;
		}

		soundManager.playSound("land");
	}
	else if (dy < 0 && !placeFree(x, y - 1, objects))
	{
		dy = 0; // Hitting head on the ceiling
		// TODO play sound?
	}

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
	if ((dy == 0.0f && !midThrow && !crouching) || jumped)
	{
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

	if (x < sprite.getOrigin().x * 2.0f) x = sprite.getOrigin().x * 2.0f;

	// Update view
	float vx = x;
	float vy = y;
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
	else if (dy > 0.0f && !jumped) setAnimation("still"); // Falling
	else if (dx != 0.0f)
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
