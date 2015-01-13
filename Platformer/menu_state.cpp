#include "menu_state.h"

Menu_State::Menu_State(StateManager *sM, TextureManager &textureManager)
{
	setStateManager(sM);
	getStateManager()->setState("level01");
}

Menu_State::~Menu_State()
{

}


/* Actions */
void Menu_State::draw(sf::RenderWindow &window)
{

}

void Menu_State::update(sf::RenderWindow &window, TextureManager &textureManager, SoundManager &soundManager, InputHandler &inputHandler)
{

}
