#include "state.h"

State::State()
{
	
}

State::~State()
{

}


// Mutators
void State::setView(sf::View v) { view = v; }
void State::setStateManager(StateManager *sM) { stateManager = sM; }


// Accesors
sf::View* State::getView() { return &view; }
sf::Clock* State::getClock() { return &clock; }
float State::getViewX()
{
	return view.getCenter().x - (WINDOW_WIDTH / 2.0f);
}
StateManager* State::getStateManager() { return stateManager; }


// Actions
void State::draw(sf::RenderWindow &window)
{

}

void State::update(sf::RenderWindow &window, TextureManager &textureManager, SoundManager &soundManager, InputHandler &inputHandler)
{

}

sf::Time State::restartClock()
{
	return clock.restart();
}
