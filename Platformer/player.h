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

#include "settings.h"
#include "texture_manager.h"
#include "sound_manager.h"
#include "damageable_object.h"
#include "weapon.h"

class ModalAnimation;

namespace PlayerWeapon { enum Enum {
	SPEAR, BOMB, TORCH, TRIDENT, HAMMER
};}

namespace PlayerArmour { enum Enum {
	NAKED, SILVER, GOLD
};}

class Player : public DamageableObject
{
	private:
		TextureManager &textureManager;
		sf::Sprite sprite;
		sf::RectangleShape rectangle;
		std::map<std::string, std::vector<sf::IntRect> > animations;
		std::string animation, texture;
		float moveSpeed, jumpSpeed, frame, throwTime;
		int jumps;
		PlayerArmour::Enum armour;
		bool jumped, midJump, midThrow, rolling, flipped, crouching, invincible, hit, dead, visible;
		sf::Clock jumpTimer, throwTimer, rollTimer, invincibleTimer, flashTimer;
		double total_time = 0.0;
		int xJumpedFrom;
		PlayerWeapon::Enum chosenWeapon;

		// Actions
		void updateAnimation(sf::Time deltaTime);
		void changeTexture(TextureManager &textureManager, std::string tex);
		void fixTexture();
		void checkDoubleJumpedObjects(Room &room);
		std::pair<int, int> getJumpPoints() const;
		Weapon* createWeaponAt(float x, float y);
		bool canThrowWeapon(Room const &room) const;
		std::unique_ptr<ModalAnimation> makeUpgradeAnimation(float xoff, float yoff, bool hasFlash, sf::Texture const &animTexture, std::vector<sf::IntRect> const &animFrames) const;
		float fixAdjXForDirection(float adjx) const;
	public:
		Player(TextureManager &textureManager, float x, float y);

		// Mutators
		void setCrouching(bool c);
		void damage(int otherX);
		void upgrade(PlayerArmour::Enum a, Room &room, settings_t const &settings);
		void setWeapon(PlayerWeapon::Enum a);
		
		// Accesors
		int getDir() const;
		int isAlive() const;
		double getFadeTime() const;
		bool getInvincible() const;
		bool isTransforming() const;
		bool isFadingOut() const;
		virtual sf::FloatRect getRect() const;

		// Actions
		virtual void draw(sf::RenderWindow &window);
		void jump(int dir, SoundManager &soundManager, const settings_t &settings);
		void throwWeapon(Room &room, int dir, SoundManager &soundManager, const settings_t &settings);
		void move(int dir);
		void update(sf::Time deltaTime, Room &room, const settings_t &settings);
		void setAnimation(std::string name);
};

#endif
