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
#include "texture_manager.h"
#include "sound_manager.h"
#include "object.h"
#include "projectile.h"

class Player
{
	private:
		sf::Sprite sprite;
		sf::RectangleShape rectangle;
		std::map<std::string, std::vector<sf::IntRect> > animations;
		std::string animation, texture;
		float x, y, dx, dy, width, height, moveSpeed, jumpSpeed, gravity, maxFallSpeed, frame, throwTime;
		int jumps;
		bool jumped, midJump, midThrow, flipped, crouching;
		void updateAnimation(sf::Time deltaTime);
		sf::Clock jumpTimer, throwTimer;
	public:
		Player(TextureManager &textureManager,float xx, float yy);

		// Mutators
		void setX(float xx);
		void setY(float yy);
		void setDX(float dxx);
		void setDY(float dyy);
		void setCrouching(bool c);
		
		// Accesors
		float getX();
		float getY();
		float getDX();
		float getDY();
		int getDir();

		sf::FloatRect getRect();
		bool placeFree(float xx, float yy, std::vector<Object*> const objects) const;

		// Actions
		void draw(sf::RenderWindow &window);
		void jump(int dir, SoundManager &soundManager);
		void throwWeapon(std::vector<Object*> &objects, int dir, TextureManager &textureManager, SoundManager &soundManager);
		void move(int dir);
		void update(sf::Time deltaTime, sf::RenderWindow &window, sf::View &view, TextureManager &textureManager, SoundManager &soundManager, std::vector<Object*> objects);
		void setAnimation(std::string name);
		void changeTexture(TextureManager &textureManager, std::string tex);
};

#endif
