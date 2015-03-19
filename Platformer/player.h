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
		sf::Sprite sprite;
		sf::RectangleShape rectangle;
		std::map<std::string, std::vector<sf::IntRect> > animations;
		std::string animation, texture;
		float moveSpeed, jumpSpeed, frame, throwTime;
		int jumps;
		bool jumped, midJump, midThrow, flipped, crouching;
		sf::Clock jumpTimer, throwTimer;
		double total_time = 0.0;

		// Actions
		void updateAnimation(sf::Time deltaTime);
	public:
		Player(TextureManager &textureManager, float x, float y);

		// Mutators
		void setCrouching(bool c);
		
		// Accesors
		int getDir() const;

		// Actions
		virtual void draw(sf::RenderWindow &window);
		void jump(int dir, SoundManager &soundManager, const settings_t &settings);
		void throwWeapon(std::vector<Object*> &objects, int dir, TextureManager &textureManager, SoundManager &soundManager, const settings_t &settings);
		void move(int dir);
		void update(sf::Time deltaTime, Room const &room, const settings_t &settings);
		void setAnimation(std::string name);
		void changeTexture(TextureManager &textureManager, std::string tex);
};

#endif
