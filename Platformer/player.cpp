#include "player.h"

#define PLAYER_WIDTH  23
#define PLAYER_HEIGHT 37
Player::Player(TextureManager &textureManager, float x, float y)
	: Object(
			Object::Type::Player,
			x, y, PLAYER_WIDTH, PLAYER_HEIGHT, // x, y, w, h
			0.0f, 0.0f,     // dx, dy
			false,           // solid
			0.0014f / 2.0f,       // Gravity
			0.2f            // Fall speed
	  ),
	  rectangle(sf::Vector2f(PLAYER_WIDTH, PLAYER_HEIGHT)),
	  animation("still"), texture("arthur1"),
	  moveSpeed(0.19f / 2.0f), jumpSpeed(0.5f / 2.0f), frame(0.0f), throwTime(0.0f),
	  jumps(0),
	  jumped(false), midJump(false), midThrow(false), flipped(false), crouching(false)
{
	// Sprite
	sprite.setTexture(textureManager.getRef(texture));
	sprite.setOrigin(14.0f, 37.0f);

	// Animations
	animations["still"].emplace_back(7, 3, 24, 37);

	animations["crouch"].emplace_back(-2, 122, 33, 35);

	animations["jumpu"].emplace_back(0, 45, 38, 37);
	animations["jumpu2"].emplace_back(34, 45, 38, 37);

	animations["jumps"].emplace_back(78, 45, 36, 37);
	animations["jumps2"].emplace_back(120, 45, 36, 37);

	animations["jumpi"].emplace_back(152, 45, 28, 37);
	
	animations["run"].emplace_back(34, 3, 28, 37);
	animations["run"].emplace_back(151, 3, 28, 37);
	animations["run"].emplace_back(64, 3, 28, 37);
	animations["run"].emplace_back(92, 3, 28, 37);
	animations["run"].emplace_back(184, 3, 28, 37);
	animations["run"].emplace_back(118, 3, 28, 37);

	animations["throw"].emplace_back(3, 83, 28, 37);
	animations["throw"].emplace_back(33, 82, 36, 37);

	animations["throwi"].push_back(animations.at("throw")[0]);
	animations["throwi"].push_back(animations.at("throw")[1]);
	animations["throwi"].push_back(animations.at("jumpi")[0]);
	animations["throwi"].push_back(animations.at("jumpi")[0]);
	animations["throwi"].emplace_back(178, 45, 26, 37);
	animations["throwi"].emplace_back(178, 45, 26, 37);
	animations["throwi"].emplace_back(202, 45, 26, 37);
	animations["throwi"].emplace_back(202, 45, 26, 37);

	animations["throwc"].emplace_back(28, 122, 33, 35);
	animations["throwc"].emplace_back(59, 122, 38, 35);
}


/* Mutators */
void Player::setCrouching(bool c)
{
	crouching = c;

	if (dy != 0.0f || jumped) crouching = false;
}


/* Accessors */
int Player::getDir() const
{
	int dir = 1;

	if (sprite.getScale().x < 0.0f) dir = -1;

	return dir;
}


/* Actions */
void Player::draw(sf::RenderWindow &window)
{
	if (DEBUG_MODE) rectangle.setPosition(roundf(x), roundf(y));
	
	int sign_scalex;
	if (sprite.getScale().x >= 0.0f) sign_scalex = 1;
	else sign_scalex = -1;

	float adjx = 0.0f, adjy = 0.0f;

	if (jumped)
	{
		if (dx == 0.0f)
		{
			if (midThrow && jumps == 2) adjx = -3.0f;
			else if (midJump) adjx = -3.0f;
			else if (midThrow) adjx = 0.0f;
			else adjx = -3.0f;
		}
		else if (midThrow) adjx = 0.0f;
		else adjx = -3.0f;
	}
	else if (crouching) { adjx = -3.0f; adjy = 6.0f; }
	else adjx = 0.0f;

	sprite.setPosition(x + sprite.getOrigin().x + (adjx * (float)sign_scalex), y + sprite.getOrigin().y + adjy);

	if (DEBUG_MODE) window.draw(rectangle);
	window.draw(sprite);
}

void Player::move(int dir)
{
	if (dy == 0 && !jumped) dx = dir * moveSpeed;
	if (dir != 0) sprite.setScale(sf::Vector2f((float)dir, 1.0f));
}

void Player::jump(int dir, SoundManager &soundManager)
{
	if (jumps < 2)
	{
		if (jumps == 0 && dy != 0.0f) return;
		if (jumps == 1)
		{
			setAnimation("jumpi");
			midJump = true;
			jumpTimer.restart();
		}

		dy = -jumpSpeed;
		dx = dir * moveSpeed;

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

		float adjx = -(float)getDir() * 8.0f, adjy = 0.0f;
		if (crouching)
		{
			adjx = -(float)getDir() * 4.0f;
			adjy = 12.0f;
		}

		Projectile* weapon = new Projectile(x + sprite.getOrigin().x + adjx, y + sprite.getOrigin().y - 35.0f + adjy, dir, textureManager);
		objects.push_back(weapon);

		soundManager.playSound("throw");
	}
}

void Player::update(sf::Time deltaTime, sf::RenderWindow &window, TextureManager &textureManager, SoundManager &soundManager, std::vector<Object*> const objects)
{
	double mstime = deltaTime.asMicroseconds() / 1000.0;
	
	if (DEBUG_MODE)
	{
		total_time += mstime;
		printf("Total Time: %4.3f\n", (float)total_time);
	}

	// Gravity
	if (placeFree(x, y + 1, objects)) dy += gravity * (float)mstime;
	else if (dy > 0.0f)
	{
		dy = 0;

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

	if (x < 0.0f) x = 0.0f;

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
