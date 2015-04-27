#ifndef PLAYER_H
#define PLAYER_H

#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <stdlib.h>
#include <math.h>
#include <string>
#include <vector>
#include <memory>
#include <map>
#include <functional>

class ModalAnimation;
class Weapon;
#include "damageable_object.h"
#include "player_armour.h"
#include "weapons/player_weapon.h"


class Player : public DamageableObject
{
	private:
		sf::Sprite sprite;
		sf::RectangleShape rectangle;
		std::map<std::string, std::vector<sf::IntRect> > animations;
		std::string animation, texture;
		float moveSpeed, jumpSpeed, frame, throwTime;
		int jumps;
		PlayerArmour::Enum armour;
		bool jumped, midJump, midThrow, rolling, flipped, crouching, invincible;
		bool hit, dead, visible, lastAttackWasMelee;
		sf::Clock jumpTimer, throwTimer, rollTimer, invincibleTimer, flashTimer;
		double total_time = 0.0;
		int xJumpedFrom;
		PlayerWeapon::Enum chosenWeapon;

		// Actions
		void updateAnimation(sf::Time deltaTime);
		void changeTexture(std::string tex);
		void fixTexture();
		void checkDoubleJumpedObjects();
		std::pair<int, int> getJumpPoints() const;
		Weapon* createWeaponAt(float x, float y);
		bool canThrowWeapon() const;
		std::unique_ptr<ModalAnimation> makeUpgradeAnimation(float xoff, float yoff, bool hasFlash, sf::Texture const &animTexture, std::vector<sf::IntRect> const &animFrames) const;
		float fixAdjXForDirection(float adjx) const;
		std::function<Weapon*(Room&, float, float, int)> getWeaponSpawnFunc(PlayerWeapon::Enum weapon, bool super);

	public:
		Player(Room &room, float x, float y);

		// Mutators
		void setCrouching(bool c);
		void damage(Object *other, int damage) override;
		void upgrade(PlayerArmour::Enum a);
		void setWeapon(PlayerWeapon::Enum a);
		
		// Accesors
		int getDir() const;
		int isAlive() const;
		bool isCrouching() const;
		double getFadeTime() const;
		bool getInvincible() const;
		bool isTransforming() const;
		bool isFadingOut() const;
		sf::Vector2f getSpriteTopLeft() const;
		float getAnimationSpeed() const;
		virtual sf::FloatRect getRect() const override;
		bool canUpgradeRightNow() const;
		bool isKnockedBack() const;

		// Actions
		virtual void draw(sf::RenderWindow &window) override;
		void jump(int dir);
		void throwWeapon(int dir);
		void move(int dir);
		void update(sf::Time deltaTime) override;
		void setAnimation(std::string name);
};

#endif
