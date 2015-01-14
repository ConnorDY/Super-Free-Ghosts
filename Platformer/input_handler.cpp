#include "input_handler.h"

InputHandler::InputHandler()
{
	// Left
	InputKeys *key = new InputKeys();
	key->inputType = Key;
	key->eventType = sf::Event::KeyPressed;
	key->keyCode = sf::Keyboard::Left;
	Inputs[Input::Left] = key;

	// Right
	key = new InputKeys();
	key->inputType = Key;
	key->eventType = sf::Event::KeyPressed;
	key->keyCode = sf::Keyboard::Right;
	Inputs[Input::Right] = key;

	// Down
	key = new InputKeys();
	key->inputType = Key;
	key->eventType = sf::Event::KeyPressed;
	key->keyCode = sf::Keyboard::Down;
	Inputs[Input::Down] = key;

	// Jump/Up
	key = new InputKeys();
	key->inputType = Key;
	key->eventType = sf::Event::KeyPressed;
	key->keyCode = sf::Keyboard::Up;
	Inputs[Input::Up] = key;

	// Throw Weapon
	key = new InputKeys();
	key->inputType = Key;
	key->eventType = sf::Event::KeyPressed;
	key->keyCode = sf::Keyboard::Z;
	Inputs[Input::Action] = key;

	// Start
	key = new InputKeys();
	key->inputType = Key;
	key->eventType = sf::Event::KeyPressed;
	key->keyCode = sf::Keyboard::Return;
	Inputs[Input::Start] = key;

	// Exit
	key = new InputKeys();
	key->inputType = Key;
	key->eventType = sf::Event::KeyPressed;
	key->keyCode = sf::Keyboard::Escape;
	Inputs[Input::Exit] = key;


	/* Debug */

	// Change Arthur's Texture
	key = new InputKeys();
	key->inputType = Key;
	key->eventType = sf::Event::KeyPressed;
	key->keyCode = sf::Keyboard::E;
	Inputs[Input::Debug0] = key;
}


/* Actions */
bool InputHandler::checkInput(Input i, sf::Event e)
{
	// Mouse event
	if (Inputs[i]->inputType == MouseInput &&
		Inputs[i]->eventType == e.type &&
		Inputs[i]->mouseButton == e.mouseButton.button) return true;
	// KeyPress event
	else if (Inputs[i]->inputType == Key &&
		Inputs[i]->eventType == e.type &&
		Inputs[i]->keyCode == e.key.code) return true;
	else return false;
}

bool InputHandler::checkInput(Input i)
{
	// KeyHeld event
	if (Inputs[i]->inputType == Key &&
		sf::Keyboard::isKeyPressed(Inputs[i]->keyCode)) return true;
	else return false;
}
