#include "state.h"

State::State(StateManager &sm) : stateManager(sm)
{

}

State::~State()
{

}


// Mutators
void State::setView(sf::View v) { view = v; }


// Accesors
sf::View& State::getView() { return view; }
sf::Clock const& State::getClock() const { return clock; }
float State::getViewX() const
{
	return view.getCenter().x - (WINDOW_WIDTH / 2.0f);
}


// Actions
void State::draw(sf::RenderWindow &window) const
{

}

void State::update(sf::RenderWindow &window, TextureManager &textureManager, SoundManager &soundManager, InputHandler &inputHandler)
{

}

sf::Time State::restartClock()
{
	return clock.restart();
}
