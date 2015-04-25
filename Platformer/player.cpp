#include <functional>
#include <stdexcept>
#include "player.h"
#include "globals.h"
#include "room.h"
#include "torch.h"
#include "spear.h"
#include "super_spear.h"
#include "hammer.h"
#include "super_hammer.h"
#include "trident.h"
#include "super_trident.h"
#include "blood_particle.h"
#include "animations/combined_animations.h"
#include "animations/dim_animation.h"
#include "animations/fade_in_animation.h"
#include "animations/flash_animation.h"
#include "animations/sprite_animation.h"

#define PLAYER_WIDTH  17
#define PLAYER_HEIGHT 35

Player::Player(Room &room, float x, float y)
	: DamageableObject(
			room, x, y, PLAYER_WIDTH, PLAYER_HEIGHT, // x, y, w, h
			0.0f, 0.0f,     // dx, dy
			false,          // solid
			0.0014f / 2.0f, // Gravity
			0.2f            // Fall speed
	  ),
	  rectangle(sf::Vector2f(PLAYER_WIDTH, PLAYER_HEIGHT)),
	  animation("still"), texture("player2"),
	  moveSpeed(0.16f / 2.0f), jumpSpeed(0.5f / 2.0f), frame(0.0f), throwTime(0.0f),
	  jumps(0), armour(PlayerArmour::SILVER),
	  jumped(false), midJump(false), midThrow(false), rolling(false), flipped(false), crouching(false), invincible(false), hit(false), dead(false), visible(true),
	  chosenWeapon(PlayerWeapon::SPEAR)
{
	// Sprite
	sprite.setTexture(room.textureManager.getRef(texture));

	// Animations
	animations["still"].emplace_back(0, 0, 50, 50);

	animations["crouch"].emplace_back(0, 150, 50, 50);

	animations["jumpu"].emplace_back(0, 200, 50, 50);
	animations["jumpu2"].emplace_back(50, 200, 50, 50);
	animations["jumpud"].emplace_back(100, 200, 50, 50);
	animations["jumpu2d"].emplace_back(150, 200, 50, 50);

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

	animations["hit"].emplace_back(250, 0, 50, 50);

	animations["roll"].emplace_back(150, 250, 50, 50);
	animations["roll"].emplace_back(200, 250, 50, 50);
	animations["roll"].emplace_back(250, 250, 50, 50);

	animations["die"].emplace_back(0, 0, 50, 50);
	animations["die"].push_back(animations.at("die")[0]);
	animations["die"].push_back(animations.at("die")[0]);
	animations["die"].push_back(animations.at("die")[0]);
	animations["die"].emplace_back(50, 0, 50, 50);
	animations["die"].emplace_back(100, 0, 50, 50);
	animations["die"].emplace_back(150, 0, 50, 50);
	animations["die"].emplace_back(200, 0, 50, 50);
	animations["die"].emplace_back(250, 0, 50, 50);
	animations["die"].emplace_back(0, 50, 50, 50);
	animations["die"].emplace_back(50, 50, 50, 50);
	animations["die"].emplace_back(100, 50, 50, 50);

	// Transformation animations
	for (unsigned int i = 0; i < 2; i++)
	{
		for (unsigned int j = 0; j < 16; j++)
		{
			animations["transform1"].emplace_back(i * 182, j * 136, 182, 136);
		}
	}

	for (unsigned int i = 0; i < 37; i++)
	{
		animations["transform2-1"].emplace_back(i * 92, 0, 92, 450);
		animations["transform2-2"].emplace_back(i * 92, 450, 92, 450);
	}
	
	// Depth and Health
	setDepth(-4);
	setHealth(2);
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
	if (armour != PlayerArmour::NAKED)
	{
		armour = PlayerArmour::NAKED;
		invincible = true;
		invincibleTimer.restart();
	}
	else
	{
		setDepth(-1);
		dead = true;
	}

	fixTexture();

	// Knock player back
	dx = dir * .1f;
	dy = -.2f;
	jumped = true;
	hit = true;
	jumps = 3;

	// Flip Player
	sprite.setScale(sf::Vector2f(sprite.getScale().x * -1, 1.0f));
}

std::unique_ptr<ModalAnimation> Player::makeUpgradeAnimation(float xoff, float yoff, bool hasFlash, sf::Texture const &animTexture, std::vector<sf::IntRect> const &animFrames) const
{
	float anim_speed = 12.5f;
	if (armour == PlayerArmour::GOLD) anim_speed = 16.5f;

	auto lastFrame = std::vector<sf::IntRect>(animFrames.end() - 1, animFrames.end());

	std::unique_ptr<ModalAnimation> spriteAnimation = std::make_unique<SpriteAnimation>(room, x + xoff, y + yoff, getDir(), animTexture, animFrames, anim_speed, this);
	if (hasFlash)
	{
		std::vector<int> flashes1 { 7, 13, 19, 25, 31 };
		std::vector<int> flashes2 { 11, 17, 23, 29, 35 };

		sf::Color flashColour1(255, 255, 255, 128), flashColour2(255, 255, 255, 64);

		spriteAnimation = CombinedAnimations::keepUntilBothFinish(
			CombinedAnimations::keepUntilBothFinish(
				std::make_unique<FlashAnimation>(room, flashes1, anim_speed, flashColour1),
				std::make_unique<FlashAnimation>(room, flashes2, anim_speed, flashColour2)
			),
			std::move(spriteAnimation)
		);
	}

	return CombinedAnimations::inSequence(
		CombinedAnimations::keepUntilBothFinish(
			std::make_unique<DimAnimation>(room, 0.2f, sf::Color(0, 0, 0, 100)),
			std::move(spriteAnimation)
		),
		CombinedAnimations::keepUntilBothFinish(
			std::make_unique<FadeInAnimation>(room, 0.2f, sf::Color(0, 0, 0, 100)),
			std::make_unique<SpriteAnimation>(room, x + xoff, y + yoff, getDir(), animTexture, lastFrame, anim_speed, this)
		)
	);
}

float Player::fixAdjXForDirection(float adjx) const
{
	if (sprite.getScale().x < 0.0f) adjx = getRect().width - adjx;
	return adjx;
}

void Player::upgrade(PlayerArmour::Enum a)
{
	invincible = true;
	auto armourLast = armour;
	armour = a;

	std::unique_ptr<ModalAnimation> animation;
	switch (armour)
	{
		case PlayerArmour::SILVER:
			animation = makeUpgradeAnimation(fixAdjXForDirection(-76.0f), -100.0f, false, room.textureManager.getRef("transform1"), animations["transform1"]);
			break;
		case PlayerArmour::GOLD:
			{
				// do transform 2
				std::vector<sf::IntRect> animFrames;
				switch (armourLast)
				{
					case PlayerArmour::NAKED:
						animFrames = animations["transform2-1"];
						break;

					case PlayerArmour::SILVER:
						animFrames = animations["transform2-2"];
						break;

					default:
						throw std::domain_error("Tried to upgrade to gold from an armour with no animation");
				}
				animation = makeUpgradeAnimation(fixAdjXForDirection(-40.0f), -415.0f, true, room.textureManager.getRef("transform2"), animFrames);
			}
			break;
		default:
			throw std::domain_error("Tried to upgrade to an armour with no animation");
	}
	room.playModalAnimation(std::move(animation));
	fixTexture();
}

void Player::setWeapon(PlayerWeapon::Enum a)
{
	chosenWeapon = a;
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

sf::Vector2f Player::getPos() const
{
	return sf::Vector2f(x, y);
}

sf::FloatRect Player::getRect() const
{
	if (crouching) return sf::FloatRect(x, y + 15.0f, width, height - 15.0f);
	if (jumps > 0)
	{
		if (armour == PlayerArmour::NAKED) return sf::FloatRect(x, y + 4.0f, width, height - 14.0f);
		return sf::FloatRect(x, y + 2.0f, width - 2.0f, height - 10.0f);
	}
	return sf::FloatRect(x, y + 2.0f, width - 2.0f, height - 2.0f);
}


/* Actions */
void Player::draw(sf::RenderWindow &window)
{
	if (!visible) sprite.setColor(sf::Color(255, 255, 255, 125));
	else sprite.setColor(sf::Color(255, 255, 255, 255));

	sf::FloatRect boundingRect = getRect();

	if (DEBUG_MODE)
	{
		rectangle.setSize(sf::Vector2f(boundingRect.width, boundingRect.height));
		rectangle.setPosition(boundingRect.left, boundingRect.top);
	}
	
	float adjx = -15.0f, adjy = -15.0f;

	if (!crouching) adjx -= 2;
	if (rolling) adjy += 7;

	adjx = fixAdjXForDirection(adjx);

	sprite.setPosition(x + adjx, y + adjy);

	if (DEBUG_MODE) window.draw(rectangle);
	window.draw(sprite);
}

void Player::move(int dir)
{
	if (dead || hit) return;

	if (dy == 0 && !jumped) dx = dir * moveSpeed;
	if (dir != 0) sprite.setScale(sf::Vector2f((float)dir, 1.0f));
}

void Player::jump(int dir)
{
	if (dead) return;

	if (jumps < 2)
	{
		if (jumps == 0)
		{
			if (dy != 0.0f) return;
			xJumpedFrom = x;
		}
		else if (jumps == 1)
		{
			setAnimation("jumpi");
			midJump = true;
			jumpTimer.restart();
		}

		dy = -jumpSpeed;
		dx = dir * moveSpeed;

		jumped = true;
		jumps++;

		room.soundManager.playSound("jump");
	}
}

std::function<Weapon*(Room&, float, float, int)> getWeaponSpawnFunc(PlayerWeapon::Enum weapon, bool super)
{
	using namespace PlayerWeapon;
	switch (weapon)
	{
		case SPEAR:
			if (super) return &SuperSpear::spawnAdjusted;
			else return &Spear::spawnAdjusted;
		case TORCH:
			return &Torch::spawnAdjusted;
		case HAMMER:
			if (super) return &SuperHammer::spawnAdjusted;
			else return &Hammer::spawnAdjusted;
		case TRIDENT:
			if (super) return &SuperTrident::spawnAdjusted;
			else return &Trident::spawnAdjusted;
		default:
			throw std::domain_error("Tried to fire a nonexistent weapon");
	}
}

Weapon* Player::createWeaponAt(float x, float y)
{
	// oh shit I think this is too many levels of abstraction for a c++ program :P
	// I did this because argument spam everywhere adds more noise than signal to the code
	return getWeaponSpawnFunc(chosenWeapon, armour == PlayerArmour::GOLD)(room, x, y, getDir());
}

bool Player::canThrowWeapon() const
{
	if (dead || hit || midThrow) return false;
	if (chosenWeapon == PlayerWeapon::TRIDENT && !Trident::canThrow(room)) return false;
	return true;
}

void Player::throwWeapon(int dir)
{
	if (!canThrowWeapon()) return;

	midThrow = true;
	throwTimer.restart();

	if (jumps == 0) throwTime = 0.14f;
	else if (jumps == 1 || armour == PlayerArmour::NAKED || flipped) throwTime = 0.13f;
	else
	{
		throwTime = 0.44f;
		flipped = true;
	}

	float adjx = -8.0f, adjy = 6.0f;
	if (crouching)
	{
		adjx -= 1.0f;
		adjy += 9.0f;
	}
	if (dir < 0) adjx = getRect().width - adjx;
	room.spawn(createWeaponAt(x + adjx, y + adjy));

	room.soundManager.playSound("throw");
}

std::pair<int, int> Player::getJumpPoints() const
{
	if (x < xJumpedFrom)
		return {x, xJumpedFrom};
	return {xJumpedFrom, x};
}

void Player::checkDoubleJumpedObjects()
{
	// Must have double-jumped
	if (jumps < 2) return;

	auto mybb = getRect();
	auto jumpLR = getJumpPoints();
	// Right hand point of our jump should be from the right of our BB, not left
	jumpLR.second += mybb.width;

	for (auto obj : room.getObjects())
	{
		auto objbb = obj->getRect();
		if (objbb.left + objbb.width > jumpLR.first && objbb.left < jumpLR.second)
			obj->onDoubleJumpedOver();
	}
}

void Player::update(sf::Time deltaTime)
{
	double mstime = deltaTime.asMicroseconds() / 1000.0;

	if (DEBUG_MODE)
	{
		total_time += mstime;
		printf("Total Time: %4.3f\n", (float)total_time);
	}

	auto& soundManager = room.soundManager;

	// Gravity
	if (placeFree(x, y + 1))
	{
		dy += gravity * (float)mstime;

		if (!rolling && armour == PlayerArmour::NAKED && jumps == 2 && dx != 0 && dy > 0 && placeFree(x, y + 1) && !placeFree(x, y + 6))
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
		else soundManager.playSound("land");
	}
	else if (dy < 0 && !placeFree(x, y - 1))
	{
		dy = 0; // Hitting head on the ceiling
		// TODO play sound?
	}

	// Update Y
	for (float i = fabs(dy) * (float)mstime; i > 0; i--)
	{
		float j = copysign(i, dy);
		if (placeFree(x, y + j))
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
				if (placeFree(x + j, y - k))
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

	pushOutOfSolids();

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

	checkDoubleJumpedObjects();

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
			else if (armour != PlayerArmour::NAKED) setAnimation("jumps2");
			else setAnimation("jumps3");
		}
		else setAnimation("run");
	}
	else
	{
		if (jumped)
		{
			if (jumps == 1)
			{
				if (armour == PlayerArmour::GOLD && dy > 0) setAnimation("jumpud");
				else setAnimation("jumpu");
			}
			else
			{
				if (armour == PlayerArmour::GOLD && dy > 0) setAnimation("jumpu2d");
				else setAnimation("jumpu2");
			}
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
		float speed = 11.5384615385f;

		frame += deltaTime.asSeconds() * speed;
		if (animation == "die" && frame > (float)(frames - 1)) frame = (float)(frames - 1);
		else frame = fmodf(frame, (float)frames); // Loop animation if it plays past "frames"
	}

	// Set TextureRect
	sprite.setTextureRect(anim[(int)frame]);
}

void Player::changeTexture(std::string tex)
{
	texture = tex;
	sprite.setTexture(room.textureManager.getRef(texture));
}

void Player::fixTexture()
{
	using namespace PlayerArmour;
	if (dead)
	{
		changeTexture("player0");
		return;
	}

	switch (armour)
	{
		case NAKED:
			changeTexture("player1");
			break;

		case SILVER:
			changeTexture("player2");
			break;

		case GOLD:
			changeTexture("player3");
			break;

		default:
			throw std::domain_error("Tried to set texture for non-existent armour");
			break;
	}
}
