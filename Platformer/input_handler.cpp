#include "input_handler.h"

InputHandler::InputHandler()
{
	// Left
	InputKeys *key = new InputKeys();
	key->inputType = Key;
	key->eventType = sf::Event::KeyPressed;
	key->keyCode = sf::Keyboard::Left;
	Keys["left"] = key;

	// Right
	key = new InputKeys();
	key->inputType = Key;
	key->eventType = sf::Event::KeyPressed;
	key->keyCode = sf::Keyboard::Right;
	Keys["right"] = key;

	// Down
	key = new InputKeys();
	key->inputType = Key;
	key->eventType = sf::Event::KeyPressed;
	key->keyCode = sf::Keyboard::Down;
	Keys["down"] = key;

	// Jump/Up
	key = new InputKeys();
	key->inputType = Key;
	key->eventType = sf::Event::KeyPressed;
	key->keyCode = sf::Keyboard::Up;
	Keys["up"] = key;

	// Throw Weapon
	key = new InputKeys();
	key->inputType = Key;
	key->eventType = sf::Event::KeyPressed;
	key->keyCode = sf::Keyboard::Z;
	Keys["action"] = key;

	// Exit
	key = new InputKeys();
	key->inputType = Key;
	key->eventType = sf::Event::KeyPressed;
	key->keyCode = sf::Keyboard::Escape;
	Keys["exit"] = key;


	/* Debug */

	// Change Arthur's Texture
	key = new InputKeys();
	key->inputType = Key;
	key->eventType = sf::Event::KeyPressed;
	key->keyCode = sf::Keyboard::E;
	Keys["debug0"] = key;
}


/* Actions */
bool InputHandler::checkInput(std::string k, sf::Event e)
{
	// Mouse event
	if (Keys.at(k)->inputType == MouseInput &&
		Keys.at(k)->eventType == e.type &&
		Keys.at(k)->mouseButton == e.mouseButton.button) return true;
	// KeyPress event
	else if (Keys.at(k)->inputType == Key &&
		Keys.at(k)->eventType == e.type &&
		Keys.at(k)->keyCode == e.key.code) return true;
	else return false;
}

bool InputHandler::checkInput(std::string k)
{
	// KeyHeld event
	if (Keys.at(k)->inputType == Key &&
		sf::Keyboard::isKeyPressed(Keys.at(k)->keyCode)) return true;
	else return false;
}
