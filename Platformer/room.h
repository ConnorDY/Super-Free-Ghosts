#ifndef ROOM_H
#define ROOM_H

class Object;
#include "state.h"

class Room : public State {
	private:
		SoundManager &soundManager;                 // Sound manager for use by objects in this room
	protected:
		std::vector<Object*> objects;               // Objects present in this room
		Object *view_follow;                        // The object to follow around with the camera
		sf::Time deltaTime;                         // The time delta between the most recent update() and the previous
		int const width, height;                    // The width and height of the room
		sf::Sprite dirtSprite, bg02, bg03, under01; // The sprite to paint the heightmap with (and decorative sprites)
		std::vector<int> heightmap;                 // Height map. Index = x-value, value = y-value (from 0 = bottom)
		                                            // However, 0 will be non-colliding for simplicity's sake
		std::vector<Object*> spawnQueue;            // Queue of objects to be spawned next tick

		void ensureHeightmapWidth(size_t width);
		void fillHeightMap(size_t xleft, size_t width, int height);
		void createSlope(size_t xleft, size_t width, int heightS, int heightE);
		void drawHeightMap(sf::RenderWindow &window);
		void drawHeightMapBack(sf::RenderWindow &window);

		void drawTree(int x, int y, sf::RenderWindow &window);
		void drawDecor(int x, int y, int type, sf::RenderWindow &window);
	public:
		explicit Room(StateManager &stm, SoundManager &som, TextureManager &tm, const settings_t &settings);
		virtual ~Room();

		bool heightmapIntersects(sf::FloatRect const &rect) const;
		int getMinTerrainYBetween(int left, int right) const;
		bool exceedsHorizontalBounds(sf::FloatRect const &rect) const;
		SoundManager& getSoundManager() const;
		std::vector<Object*> const getObjects() const;
		void spawn(Object *obj);
		virtual void start(TextureManager &textureManager, const settings_t &settings) = 0;
		virtual void end();
		void reset(TextureManager &textureManager, const settings_t &settings);
		virtual void updateView(sf::RenderWindow &window);
		virtual void drawBackground(sf::RenderWindow &window);
		virtual void drawSprites(sf::RenderWindow &window);
		virtual void drawForeground(sf::RenderWindow &window);
		virtual void draw(sf::RenderWindow &window);
		virtual void update(sf::RenderWindow &window, TextureManager &textureManager, SoundManager &soundManager, InputHandler &inputHandler, const settings_t &settings);
};

#endif
