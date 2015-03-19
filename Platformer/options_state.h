#ifndef OPTIONS_STATE_H
#define OPTIONS_STATE_H

#include <SFML/Audio.hpp>
#include <vector>

#include "globals.h"
#include "settings.h"
#include "state.h"

class Options_State : public State
{
private:
	std::vector<std::string> menuOptions;
	std::vector<std::vector<std::string>> optionChoices;
	std::vector<unsigned int> currentValue;
	sf::Font fnt;
	unsigned int currentOption = 0;
public:
	Options_State(StateManager &sM, TextureManager &textureManager);
	virtual ~Options_State();

	// Actions
	virtual void draw(sf::RenderWindow &window);
	virtual void update(sf::RenderWindow &window, TextureManager &textureManager, SoundManager &soundManager, InputHandler &inputHandler, settings_t &settings);
};

#endif
