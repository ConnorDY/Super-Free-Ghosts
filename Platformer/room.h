#ifndef ROOM_H
#define ROOM_H

class Object;
class ModalAnimation;
#include "state.h"

class Room : public State {
	private:
		std::unique_ptr<ModalAnimation> animation;  // Currently-playing modal animation (or else wrapped nullptr)

		bool shouldDraw(Object const* object) const;
	protected:
		std::vector<Object*> objects;               // Objects present in this room
		Object *view_follow;                        // The object to follow around with the camera
		sf::Time deltaTime;                         // The time delta between the most recent update() and the previous
		int const width, height;                    // The width and height of the room
		sf::Sprite dirtSprite, bg02, bg03, under01; // The sprite to paint the heightmap with (and decorative sprites)
		std::vector<int> heightmap;                 // Height map. Index = x-value, value = y-value (from 0 = bottom)
		                                            // However, 0 will be non-colliding for simplicity's sake
		std::vector<Object*> spawnQueue;            // Queue of objects to be spawned next tick
		settings_t &settings;

		void ensureHeightmapWidth(size_t width);
		void fillHeightMap(size_t xleft, size_t width, int height);
		void createSlope(size_t xleft, size_t width, int heightS, int heightE);
		void drawHeightMap(sf::RenderWindow &window);
		void drawHeightMapBack(sf::RenderWindow &window);

		void drawTree(int x, int y, sf::RenderWindow &window);
		void drawDecor(int x, int y, int type, sf::RenderWindow &window);
	public:
		SoundManager &soundManager;                 // Sound manager for use by objects in this room
		TextureManager const &textureManager;

		explicit Room(StateManager &stm, SoundManager &som, TextureManager const &tm, settings_t &settings);
		virtual ~Room();

		bool heightmapIntersects(sf::FloatRect const &rect) const;
		int getMinTerrainYBetween(int left, int right) const;
		bool exceedsHorizontalBounds(sf::FloatRect const &rect) const;

		settings_t const& getSettings() const;
		std::vector<Object*> const getObjects() const;
		bool isAnimationInProgress() const;

		void spawn(Object *obj);
		void playModalAnimation(std::unique_ptr<ModalAnimation> animation);

		virtual void start() = 0;
		virtual void end();
		void reset();

		virtual void updateView(sf::RenderWindow &window);
		virtual void drawBackground(sf::RenderWindow &window);
		virtual void drawSprites(sf::RenderWindow &window);
		virtual void drawForeground(sf::RenderWindow &window);
		virtual void draw(sf::RenderWindow &window) override;
		virtual void update(sf::RenderWindow &window, SoundManager &soundManager, InputHandler &inputHandler) override;
};

#endif
