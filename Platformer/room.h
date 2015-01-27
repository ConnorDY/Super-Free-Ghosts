#ifndef ROOM_H
#define ROOM_H

class Object;

#include "state.h"
#include "object.h"

class Room : public State {
	private:
		SoundManager &soundManager;   // Sound manager for use by objects in this room
	protected:
		std::vector<Object*> objects; // Objects present in this room
		Object *view_follow;          // The object to follow around with the camera
		sf::Time deltaTime;           // The time delta between the most recent update() and the previous
		int const width, height;      // The width and height of the room
		sf::Sprite dirtSprite;        // The sprite to paint the heightmap with
		std::vector<int> heightmap;   // Height map. Index = x-value, value = y-value (from 0 = bottom)
		                              // However, 0 will be non-colliding for simplicity's sake

		void fillHeightMap(size_t xleft, size_t width, int height);
		void drawHeightMap(sf::RenderWindow &window);
	public:
		explicit Room(StateManager &stm, SoundManager &som, TextureManager &tm);
		virtual ~Room();

		bool heightmapIntersects(sf::FloatRect const &rect) const;
		bool exceedsHorizontalBounds(sf::FloatRect const &rect) const;
		SoundManager& getSoundManager() const;
		std::vector<Object*> const getObjects() const;
		virtual void start(TextureManager &textureManager) = 0;
		virtual void end();
		void reset(TextureManager &textureManager);
		virtual void draw(sf::RenderWindow &window);
		virtual void update(sf::RenderWindow &window, TextureManager &textureManager, SoundManager &soundManager, InputHandler &inputHandler);
};

#endif
