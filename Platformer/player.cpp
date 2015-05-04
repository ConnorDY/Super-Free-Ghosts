#include <functional>
#include <stdexcept>
#include "player.h"
#include "globals.h"
#include "room.h"
#include "blood_particle.h"
#include "weapons/weapon.h"
#include "animations/combined_animations.h"
#include "animations/dim_animation.h"
#include "animations/fade_in_animation.h"
#include "animations/flash_animation.h"
#include "animations/sprite_animation.h"

/*
	So I don't forget:
	Our main character's name is Alexander
*/

namespace {
	int const PLAYER_WIDTH = 17, PLAYER_HEIGHT = 35;
	float const ANIMATION_SPEED = 11.5384615385f;

	struct Animations
	{
		std::vector<sf::IntRect>
			STILL, CROUCH, FIRST_JUMP_UP, SECOND_JUMP_UP,
			FIRST_JUMP_DOWN, SECOND_JUMP_DOWN, DIE, MELEE,
			FIRST_JUMP_SIDEWAYS, SECOND_JUMP_SIDEWAYS_NAKED,
			SECOND_JUMP_SIDEWAYS_ARMOURED, DOUBLE_JUMP_BEGIN,
			RUN, THROW, THROW_DOUBLE_JUMPING, ROLL, HURT,
			THROW_CROUCHING, MELEE_CROUCHING, TRANSFORM_SILVER,
			TRANSFORM_NAKED_GOLD, TRANSFORM_SILVER_GOLD;

		Animations()
		{
			STILL.emplace_back(0, 0, 50, 50);

			CROUCH.emplace_back(0, 150, 50, 50);

			FIRST_JUMP_UP.emplace_back(0, 200, 50, 50);
			SECOND_JUMP_UP.emplace_back(50, 200, 50, 50);
			FIRST_JUMP_DOWN.emplace_back(100, 200, 50, 50);
			SECOND_JUMP_DOWN.emplace_back(150, 200, 50, 50);

			FIRST_JUMP_SIDEWAYS.emplace_back(0, 250, 50, 50);
			SECOND_JUMP_SIDEWAYS_ARMOURED.emplace_back(50, 250, 50, 50);
			SECOND_JUMP_SIDEWAYS_NAKED.emplace_back(100, 250, 50, 50);

			DOUBLE_JUMP_BEGIN.emplace_back(0, 300, 50, 50);

			RUN.emplace_back(0, 50, 50, 50);
			RUN.emplace_back(50, 50, 50, 50);
			RUN.emplace_back(100, 50, 50, 50);
			RUN.emplace_back(150, 50, 50, 50);
			RUN.emplace_back(200, 50, 50, 50);
			RUN.emplace_back(250, 50, 50, 50);

			THROW.emplace_back(0, 100, 50, 50);
			THROW.emplace_back(50, 100, 50, 50);

			THROW_DOUBLE_JUMPING.push_back(THROW[0]);
			THROW_DOUBLE_JUMPING.push_back(THROW[1]);
			THROW_DOUBLE_JUMPING.push_back(DOUBLE_JUMP_BEGIN[0]);
			THROW_DOUBLE_JUMPING.push_back(DOUBLE_JUMP_BEGIN[0]);
			THROW_DOUBLE_JUMPING.emplace_back(50, 300, 50, 50);
			THROW_DOUBLE_JUMPING.emplace_back(50, 300, 50, 50);
			THROW_DOUBLE_JUMPING.emplace_back(100, 300, 50, 50);
			THROW_DOUBLE_JUMPING.emplace_back(100, 300, 50, 50);

			THROW_CROUCHING.emplace_back(50, 150, 50, 50);
			THROW_CROUCHING.emplace_back(100, 150, 50, 50);

			MELEE.emplace_back(200, 100, 50, 50);
			MELEE.emplace_back(250, 100, 50, 50);
			MELEE.emplace_back(250, 100, 50, 50);

			MELEE_CROUCHING.emplace_back(200, 150, 50, 50);
			MELEE_CROUCHING.emplace_back(250, 150, 50, 50);
			MELEE_CROUCHING.emplace_back(250, 150, 50, 50);

			HURT.emplace_back(250, 0, 50, 50);

			ROLL.emplace_back(150, 250, 50, 50);
			ROLL.emplace_back(200, 250, 50, 50);
			ROLL.emplace_back(250, 250, 50, 50);

			DIE.emplace_back(0, 0, 50, 50);
			DIE.push_back(DIE[0]);
			DIE.push_back(DIE[0]);
			DIE.push_back(DIE[0]);
			DIE.emplace_back(50, 0, 50, 50);
			DIE.emplace_back(100, 0, 50, 50);
			DIE.emplace_back(150, 0, 50, 50);
			DIE.emplace_back(200, 0, 50, 50);
			DIE.emplace_back(250, 0, 50, 50);
			DIE.emplace_back(0, 50, 50, 50);
			DIE.emplace_back(50, 50, 50, 50);
			DIE.emplace_back(100, 50, 50, 50);

			// Transformation animations
			for (unsigned int i = 0; i < 2; i++)
			{
				for (unsigned int j = 0; j < 16; j++)
				{
					TRANSFORM_SILVER.emplace_back(i * 182, j * 136, 182, 136);
				}
			}

			for (unsigned int i = 0; i < 37; i++)
			{
				TRANSFORM_NAKED_GOLD.emplace_back(i * 92, 0, 92, 450);
				TRANSFORM_SILVER_GOLD.emplace_back(i * 92, 450, 92, 450);
			}
		}
	};
	Animations ANIM;
}

Player::Player(Room &room, float x, float y)
	: DamageableObject(
			room, x, y, PLAYER_WIDTH, PLAYER_HEIGHT, // x, y, w, h
			0.0f, 0.0f,     // dx, dy
			false,          // solid
			0.0014f / 2.0f, // Gravity
			0.2f            // Fall speed
	  ),
	  rectangle(sf::Vector2f(PLAYER_WIDTH, PLAYER_HEIGHT)),
	  animation(&ANIM.STILL),
	  moveSpeed(0.16f / 2.0f), jumpSpeed(0.5f / 2.0f), frame(0.0f), throwTime(0.0f),
	  jumps(0), armour(PlayerArmour::SILVER),
	  jumped(false), midJump(false), midThrow(false), rolling(false), flipped(false), crouching(false), invincible(false), hit(false), dead(false), visible(true),
	  chosenWeapon(PlayerWeapon::TRIDENT)
{
	fixTexture();
	
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

void Player::damage(Object *other, int damage)
{
	if (invincible || dead) return;

	if (damage != 0)
	{
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
	}

	// Direction to knock back
	int dir = -1;
	auto otherRect = other->getRect();
	auto myRect = getRect();
	if (myRect.left + myRect.width / 2 > otherRect.left + otherRect.width / 2)
		dir = 1;

	// Knock player back
	dx = dir * .1f;
	dy = -.2f;
	jumped = true;
	hit = true;
	jumps = 3;

	// Flip Player
	sprite.setScale(sf::Vector2f(dir, 1.0f));
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

bool Player::canUpgradeRightNow() const
{
	return !hit && !dead;
}

bool Player::isKnockedBack() const
{
	return hit;
}

void Player::upgrade(PlayerArmour::Enum a)
{
	if (dead) return;
	invincible = true;
	auto armourLast = armour;
	armour = a;

	std::unique_ptr<ModalAnimation> animation;
	switch (armour)
	{
		case PlayerArmour::SILVER:
			animation = makeUpgradeAnimation(fixAdjXForDirection(-76.0f), -100.0f, false, room.textureManager.getRef("transform-silver"), ANIM.TRANSFORM_SILVER);
			break;
		case PlayerArmour::GOLD:
			{
				// do transform 2
				std::vector<sf::IntRect> animFrames;
				switch (armourLast)
				{
					case PlayerArmour::NAKED:
						animFrames = ANIM.TRANSFORM_NAKED_GOLD;
						break;

					case PlayerArmour::SILVER:
						animFrames = ANIM.TRANSFORM_SILVER_GOLD;
						break;

					default:
						throw std::domain_error("Tried to upgrade to gold from an armour with no animation");
				}
				animation = makeUpgradeAnimation(fixAdjXForDirection(-40.0f), -415.0f, true, room.textureManager.getRef("transform-gold"), animFrames);
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

bool Player::isCrouching() const
{
	return crouching;
}

int Player::isAlive() const
{
	return !dead;
}

bool Player::getInvincible() const
{
	return invincible;
}

float Player::getAnimationSpeed() const
{
	return ANIMATION_SPEED;
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

sf::Vector2f Player::getSpriteTopLeft() const
{
	float adjx = -15.0f, adjy = -15.0f;

	if (!crouching) adjx -= 2;
	if (rolling) adjy += 7;

	adjx = fixAdjXForDirection(adjx);

	return {x + adjx, y + adjy};
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

	auto spritePos = getSpriteTopLeft();
	sprite.setPosition(spritePos.x, spritePos.y);

	if (DEBUG_MODE) window.draw(rectangle);
	window.draw(sprite);
}

void Player::move(int dir)
{
	if (dead || hit || midThrow) return;

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
			if (midThrow && lastAttackWasMelee) return;
			xJumpedFrom = x;
		}
		else if (jumps == 1)
		{
			setAnimation(ANIM.DOUBLE_JUMP_BEGIN);
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

bool Player::canThrowWeapon() const
{
	if (dead || hit || midThrow) return false;
	if (!PlayerWeapon::canThrow(room, chosenWeapon)) return false;
	return true;
}

void Player::throwWeapon(int dir)
{
	if (!canThrowWeapon()) return;

	midThrow = true;
	throwTimer.restart();
	lastAttackWasMelee = PlayerWeapon::isMeleeWeapon(chosenWeapon);

	if (lastAttackWasMelee) throwTime = 3.0f / getAnimationSpeed();
	else if (jumps == 0) throwTime = 0.14f;
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

	bool isSuper = armour == PlayerArmour::GOLD;
	auto weapon = PlayerWeapon::spawnWeapon(room, this, chosenWeapon, isSuper, x + adjx, y + adjy, getDir());
	weapon->doNotDamage(this);
	room.spawn(weapon);

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
	if (jumps < 2 || hit) return;

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
	if (DEBUG_MODE)
	{
		total_time += deltaTime.asSeconds();
		printf("Total Time: %4.3f\n", (float)total_time);
	}

	auto& soundManager = room.soundManager;

	auto oldDy = dy;
	applyGravityOnce(deltaTime);
	if (dy == 0 && oldDy > 0)
	{
		// Landed on the ground
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
	else
	{
		if (!rolling && armour == PlayerArmour::NAKED && jumps == 2 && dx != 0 && dy > 0 && placeFree(x, y + 1) && !placeFree(x, y + 6))
		{
			rolling = true;
			rollTimer.restart();
		}

		if (dy < 0 && !placeFree(x, y - 1))
		{
			dy = 0; // Hitting head on the ceiling
			// TODO play sound?
		}
	}

	auto actual_dx = dx;
	if (!jumped && (dy != 0.0f || midThrow || crouching))
		actual_dx = 0.0f; // Disallow manual x movement if crouching/throwing/falling

	applyVelocityOnce(deltaTime, actual_dx, dy);
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
	if (dead) setAnimation(ANIM.DIE);
	else if (hit) setAnimation(ANIM.HURT);
	else if (rolling) setAnimation(ANIM.ROLL);
	else if (crouching)
	{
		if (midThrow) {
			if (lastAttackWasMelee)
				setAnimation(ANIM.MELEE_CROUCHING);
			else
				setAnimation(ANIM.THROW_CROUCHING);
		}
		else setAnimation(ANIM.CROUCH);
	}
	else if (midThrow)
	{
		if (lastAttackWasMelee)
			setAnimation(ANIM.MELEE);
		else if (jumps < 2)
		{
			setAnimation(ANIM.THROW);
		}
		else setAnimation(ANIM.THROW_DOUBLE_JUMPING);
	}
	else if (dy > 0.0f && !jumped) setAnimation(ANIM.STILL); // Falling
	else if (dx != 0.0f)
	{
		if (jumped)
		{
			if (jumps == 1) setAnimation(ANIM.FIRST_JUMP_SIDEWAYS);
			else if (armour == PlayerArmour::NAKED) setAnimation(ANIM.SECOND_JUMP_SIDEWAYS_NAKED);
			else setAnimation(ANIM.SECOND_JUMP_SIDEWAYS_ARMOURED);
		}
		else setAnimation(ANIM.RUN);
	}
	else
	{
		if (jumped)
		{
			if (jumps == 1)
			{
				if (armour == PlayerArmour::GOLD && dy > 0)
					setAnimation(ANIM.FIRST_JUMP_DOWN);
				else setAnimation(ANIM.FIRST_JUMP_UP);
			}
			else
			{
				if (armour == PlayerArmour::GOLD && dy > 0)
					setAnimation(ANIM.SECOND_JUMP_DOWN);
				else setAnimation(ANIM.SECOND_JUMP_UP);
			}
		}
		else setAnimation(ANIM.STILL);
	}

	updateAnimation(deltaTime);
}

void Player::setAnimation(std::vector<sf::IntRect> const &newAnim)
{
	if (!midJump && animation != &newAnim)
	{
		frame = 0.0f;
		animation = &newAnim;
	}
}

void Player::updateAnimation(sf::Time deltaTime)
{
	int frames = animation->size();

	// Adjust frame
	if (frames > 1)
	{
		frame += deltaTime.asSeconds() * ANIMATION_SPEED;
		if (animation == &ANIM.DIE && frame > (float)(frames - 1)) frame = (float)(frames - 1);
		else frame = fmodf(frame, (float)frames); // Loop animation if it plays past "frames"
	}

	// Set TextureRect
	sprite.setTextureRect((*animation)[(int)frame]);
}

void Player::changeTexture(std::string tex)
{
	sprite.setTexture(room.textureManager.getRef(tex));
}

void Player::fixTexture()
{
	using namespace PlayerArmour;
	if (dead)
	{
		changeTexture("player-dead");
		return;
	}

	switch (armour)
	{
		case NAKED:
			changeTexture("player-naked");
			break;

		case SILVER:
			changeTexture("player-silver");
			break;

		case GOLD:
			changeTexture("player-gold");
			break;

		default:
			throw std::domain_error("Tried to set texture for non-existent armour");
			break;
	}
}
