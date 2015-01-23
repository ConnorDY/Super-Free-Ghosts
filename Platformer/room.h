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
	public:
		explicit Room(StateManager &stm, SoundManager &som);
		virtual ~Room();

		SoundManager& getSoundManager() const;
		std::vector<Object*> const getObjects() const;
		virtual void start(TextureManager &textureManager) = 0;
		virtual void end();
		void reset(TextureManager &textureManager);
		virtual void draw(sf::RenderWindow &window);
		virtual void update(sf::RenderWindow &window, TextureManager &textureManager, SoundManager &soundManager, InputHandler &inputHandler);
};

#endif
