#ifndef PLAYER_H
#define PLAYER_H

#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <stdlib.h>
#include <math.h>
#include <string>
#include <vector>
#include <map>

#include "globals.h"
#include "settings.h"
#include "texture_manager.h"
#include "sound_manager.h"
#include "object.h"
#include "projectile.h"

class Player : public Object
{
	private:
		TextureManager &textureManager;
		sf::Sprite sprite;
		sf::RectangleShape rectangle;
		std::map<std::string, std::vector<sf::IntRect> > animations;
		std::string animation, texture;
		float moveSpeed, jumpSpeed, frame, throwTime;
		int jumps, armour;
		bool jumped, midJump, midThrow, flipped, crouching, invincible, hit, dead, visible;
		sf::Clock jumpTimer, throwTimer, invincibleTimer, flashTimer;
		double total_time = 0.0;

		// Actions
		void updateAnimation(sf::Time deltaTime);
		void changeTexture(TextureManager &textureManager, std::string tex);
	public:
		Player(TextureManager &textureManager, float x, float y);

		// Mutators
		void setCrouching(bool c);
		void damage(int otherX);
		
		// Accesors
		int getDir() const;
		int isAlive() const;
		bool getInvincible() const;

		// Actions
		virtual void draw(sf::RenderWindow &window);
		void jump(int dir, SoundManager &soundManager, const settings_t &settings);
		void throwWeapon(std::vector<Object*> &objects, int dir, SoundManager &soundManager, const settings_t &settings);
		void move(int dir);
		void update(sf::Time deltaTime, Room const &room, const settings_t &settings);
		void setAnimation(std::string name);
};

#endif
