#include "state_manager.h"

#include "menu_state.h"
#include "level01_state.h"

StateManager::StateManager(TextureManager *tM, SoundManager *sM)
{
	textureManager = tM;
	soundManager = sM;

	currentState = "menu";

	// Load First State
	loadState(currentState);
}

StateManager::~StateManager()
{
	for (auto state_kv : states)
		delete state_kv.second;
}


/* Actions */
void StateManager::draw(sf::RenderWindow &window)
{
	if (states.find(currentState) != states.end()) states.at(currentState)->draw(window);
}

void StateManager::update(sf::RenderWindow &window)
{
	if (states.find(currentState) != states.end()) states.at(currentState)->update(window, *textureManager, *soundManager, inputHandler);
}

void StateManager::loadState(std::string state)
{
	if (state == "menu") states[state] = new Menu_State(this, *textureManager);
	else if (state == "level01") states[state] = new Level01_State(this, *textureManager);
}

void StateManager::unloadState(std::string state)
{
	if (states.find(state) != states.end())
	{
		delete states.at(state);
		states.erase(states.find(state));
	}
}

void StateManager::setState(std::string state)
{
	unloadState(currentState);
	currentState = state;
	loadState(currentState);
}
