#include "state_manager.h"

#include "menu_state.h"

StateManager::StateManager(TextureManager &tM, SoundManager &sM, settings_t &stg)
	: textureManager(tM), soundManager(sM), settings(stg), currentState(new Menu_State(*this, tM, stg))
{
}

StateManager::~StateManager()
{
}


/* Actions */
void StateManager::draw(sf::RenderWindow &window)
{
	currentState->draw(window);
}

void StateManager::update(sf::RenderWindow &window)
{
	currentState->update(window, textureManager, soundManager, inputHandler, settings);
}

void StateManager::setState(std::unique_ptr<State> state)
{
	currentState = std::move(state);
}
