#ifndef OPTIONS_STATE_H
#define OPTIONS_STATE_H

#include <SFML/Audio.hpp>
#include <vector>

#include "settings.h"
#include "state.h"

class Options_State : public State
{
private:
	TextureManager &textureManager;
	settings_t &settings;
	std::vector<std::string> menuOptions;
	std::vector<std::vector<std::string>> optionChoices;
	std::vector<unsigned int> currentValue;
	sf::Font fnt;
	unsigned int currentOption = 0;

	void toggleVsync(sf::RenderWindow &window);
	void toggleMusic();
	void toggleSound();
public:
	Options_State(StateManager &sM, TextureManager &tm, settings_t &settings);
	virtual ~Options_State();

	// Actions
	virtual void draw(sf::RenderWindow &window) override;
	virtual void update(sf::RenderWindow &window, SoundManager &soundManager, InputHandler &inputHandler) override;
};

#endif
