#include "player.h"

#define PLAYER_WIDTH  17
#define PLAYER_HEIGHT 35


Player::Player(TextureManager &tm, float x, float y)
	: Object(
			x, y, PLAYER_WIDTH, PLAYER_HEIGHT, // x, y, w, h
			0.0f, 0.0f,     // dx, dy
			false,           // solid
			0.0014f / 2.0f,       // Gravity
			0.2f            // Fall speed
	  ),
	  rectangle(sf::Vector2f(PLAYER_WIDTH, PLAYER_HEIGHT)),
	  animation("still"), texture("player1"),
	  moveSpeed(0.19f / 2.0f), jumpSpeed(0.5f / 2.0f), frame(0.0f), throwTime(0.0f),
	  jumps(0), armour(1),
	  jumped(false), midJump(false), midThrow(false), rolling(false), flipped(false), crouching(false), invincible(false), hit(false), dead(false), visible(true),
	  textureManager(tm)
{
	// Sprite
	sprite.setTexture(textureManager.getRef(texture));

	// Animations
	animations["still"].emplace_back(0, 0, 50, 50);

	animations["crouch"].emplace_back(0, 150, 50, 50);

	animations["jumpu"].emplace_back(0, 200, 50, 50);
	animations["jumpu2"].emplace_back(50, 200, 50, 50);

	animations["jumps"].emplace_back(0, 250, 50, 50);
	animations["jumps2"].emplace_back(50, 250, 50, 50);
	animations["jumps3"].emplace_back(100, 250, 50, 50);

	animations["jumpi"].emplace_back(0, 300, 50, 50);
	
	animations["run"].emplace_back(0, 50, 50, 50);
	animations["run"].emplace_back(50, 50, 50, 50);
	animations["run"].emplace_back(100, 50, 50, 50);
	animations["run"].emplace_back(150, 50, 50, 50);
	animations["run"].emplace_back(200, 50, 50, 50);
	animations["run"].emplace_back(250, 50, 50, 50);

	animations["throw"].emplace_back(0, 100, 50, 50);
	animations["throw"].emplace_back(50, 100, 50, 50);

	animations["throwi"].push_back(animations.at("throw")[0]);
	animations["throwi"].push_back(animations.at("throw")[1]);
	animations["throwi"].push_back(animations.at("jumpi")[0]);
	animations["throwi"].push_back(animations.at("jumpi")[0]);
	animations["throwi"].emplace_back(50, 300, 50, 50);
	animations["throwi"].emplace_back(50, 300, 50, 50);
	animations["throwi"].emplace_back(100, 300, 50, 50);
	animations["throwi"].emplace_back(100, 300, 50, 50);

	animations["throwc"].emplace_back(50, 150, 50, 50);
	animations["throwc"].emplace_back(100, 150, 50, 50);

	animations["hit"].emplace_back(6, 156, 36, 32);

	animations["roll"].emplace_back(150, 250, 50, 50);
	animations["roll"].emplace_back(200, 250, 50, 50);
	animations["roll"].emplace_back(250, 250, 50, 50);

	animations["die"].push_back(animations.at("hit")[0]);
	animations["die"].emplace_back(46, 156, 36, 32);
	animations["die"].emplace_back(87, 156, 36, 32);
	animations["die"].emplace_back(129, 156, 42, 32);
	animations["die"].emplace_back(36, 188, 42, 24);
	animations["die"].emplace_back(84, 188, 42, 24);

	// Depth
	setDepth(-2);
}


/* Mutators */
void Player::setCrouching(bool c)
{
	if (dead) return;

	crouching = c;

	if (dy != 0.0f || jumped) crouching = false;
}

void Player::damage(int otherX)
{
	if (invincible || dead) return;

	// Direction to knock back
	int dir = -1;
	if (x > otherX) dir = 1;
	
	// Decrease armour
	armour--;

	switch (armour)
	{
		default:
			dead = true;
			break;

		case 0:
			changeTexture(textureManager, "player0");
			break;
	}

	// Give invincibility if not dead
	if (armour >= 0)
	{
		invincible = true;
		invincibleTimer.restart();
	}

	// Knock player back
	dx = dir * .1f;
	dy = -.2f;
	jumped = true;
	hit = true;
	jumps = 3;

	// Flip Player
	sprite.setScale(sf::Vector2f(sprite.getScale().x * -1, 1.0f));
}


/* Accessors */
int Player::getDir() const
{
	int dir = 1;

	if (sprite.getScale().x < 0.0f) dir = -1;

	return dir;
}

int Player::isAlive() const
{
	return !dead;
}

bool Player::getInvincible() const
{
	return invincible;
}


/* Actions */
void Player::draw(sf::RenderWindow &window)
{
	if (!visible) return;

	if (DEBUG_MODE) rectangle.setPosition(x, y);
	
	int sign_scalex = 1;
	float adjx = -15.0f, adjx2 = 0.0f, adjy = -15.0f;

	if (sprite.getScale().x < 0.0f)
	{
		sign_scalex = -1;
		adjx2 = 47.0f;
	}

	if (rolling) adjy += 7;

	sprite.setPosition(x + adjx + adjx2, y + adjy);

	if (DEBUG_MODE) window.draw(rectangle);
	window.draw(sprite);
}

void Player::move(int dir)
{
	if (dead) return;

	if (dy == 0 && !jumped) dx = dir * moveSpeed;
	if (dir != 0) sprite.setScale(sf::Vector2f((float)dir, 1.0f));
}

void Player::jump(int dir, SoundManager &soundManager, const settings_t &settings)
{
	if (dead) return;

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

		if (settings.sound_on) soundManager.playSound("jump");
	}
}

void Player::throwWeapon(std::vector<Object*> &objects, int dir, SoundManager &soundManager, const settings_t &settings)
{
	if (dead || hit) return;

	if (!midThrow)
	{
		midThrow = true;
		throwTimer.restart();

		if (jumps == 0) throwTime = 0.14f;
		else if (jumps == 1 || armour == 0 || flipped) throwTime = 0.13f;
		else
		{
			throwTime = 0.44f;
			flipped = true;
		}

		float adjx = -(float)getDir() * 8.0f, adjy = 0.0f;
		if (crouching)
		{
			adjx = -(float)getDir() * 4.0f;
			adjy = 9.0f;
		}

		if (getDir() < 0) adjx -= 30;

		Projectile* weapon = new Projectile(x + adjx, y + 6.0f + adjy, dir, textureManager);
		objects.push_back(weapon);

		if (settings.sound_on) soundManager.playSound("throw");
	}
}

void Player::update(sf::Time deltaTime, Room const &room, const settings_t &settings)
{
	double mstime = deltaTime.asMicroseconds() / 1000.0;

	if (DEBUG_MODE)
	{
		total_time += mstime;
		printf("Total Time: %4.3f\n", (float)total_time);
	}

	auto& soundManager = room.getSoundManager();

	// Gravity
	if (placeFree(x, y + 1, room))
	{
		dy += gravity * (float)mstime;

		if (!rolling && armour == 0 && jumps == 2 && dx != 0 && dy > 0 && placeFree(x, y + 1, room) && !placeFree(x, y + 6, room))
		{
			rolling = true;
			rollTimer.restart();
		}
	}
	else if (dy > 0.0f)
	{
		dy = 0;

		if (jumped)
		{
			if (hit)
			{
				if (!dead) sprite.setScale(sf::Vector2f(sprite.getScale().x * -1, 1.0f));
				hit = false;
			}

			jumped = false;
			flipped = false;
			jumps = 0;
		}

		if (dead) dx = 0;
		else if (settings.sound_on) soundManager.playSound("land");
	}
	else if (dy < 0 && !placeFree(x, y - 1, room))
	{
		dy = 0; // Hitting head on the ceiling
		// TODO play sound?
	}

	// Update Y
	for (float i = fabs(dy) * (float)mstime; i > 0; i--)
	{
		float j = copysign(i, dy);
		if (placeFree(x, y + j, room))
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
			bool brk = false;

			float ks = 0;
			float ke = 0;

			if (dy == 0)
			{
				ks = -4;
				ke = 4;
			}

			for (float k = ks; k <= ke; k++)
			{
				if (placeFree(x + j, y - k, room))
				{
					x += j;
					y -= k;

					brk = true;
					break;
				}
			}

			if (brk) break;
		}
	}

	// Move out of heightmap if stuck within it
	while (room.heightmapIntersects(sf::FloatRect(x, y, width, height))) y -= .5;

	// Jump, Throw, Roll, and Invicibility Timers
	if (midJump && jumpTimer.getElapsedTime().asSeconds() >= .2) midJump = false;
	else if (midThrow && throwTimer.getElapsedTime().asSeconds() >= throwTime) midThrow = false;
	if (rolling && rollTimer.getElapsedTime().asSeconds() >= .22) rolling = false;
	if (invincible && invincibleTimer.getElapsedTime().asSeconds() >= 2.) invincible = false;

	// Invincibility
	if (invincible && flashTimer.getElapsedTime().asMilliseconds() >= 50)
	{
		flashTimer.restart();
		visible = !visible;
	}
	else if (!invincible) visible = true;

	// Animation
	if (dead) setAnimation("die");
	else if (hit) setAnimation("hit");
	else if (rolling) setAnimation("roll");
	else if (crouching)
	{
		if (midThrow) setAnimation("throwc");
		else setAnimation("crouch");
	}
	else if (midThrow)
	{
		if (jumps < 2)
		{
			if (animation == "throwi") throwTime = 0.13f;
			setAnimation("throw");
		}
		else setAnimation("throwi");
	}
	else if (dy > 0.0f && !jumped) setAnimation("still"); // Falling
	else if (dx != 0.0f)
	{
		if (jumped)
		{
			if (jumps == 1) setAnimation("jumps");
			else if (armour > 0) setAnimation("jumps2");
			else setAnimation("jumps3");
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
		frame = 0.0f;
		animation = name;
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

		frame += deltaTime.asSeconds() * speed;
		if (animation == "die" && frame > 5) frame = 5;
		else frame = fmodf(frame, (float)frames); // Loop animation if it plays past "frames"
	}

	// Set TextureRect
	sprite.setTextureRect(anim[(int)frame]);
}

void Player::changeTexture(TextureManager &textureManager, std::string tex)
{
	texture = tex;
	sprite.setTexture(textureManager.getRef(texture));
}
