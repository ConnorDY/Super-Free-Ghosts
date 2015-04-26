#include "input_handler.h"

InputHandler::InputHandler()
{
	setKey(Input::Left, sf::Keyboard::Left);
	setKey(Input::Right, sf::Keyboard::Right);
	setKey(Input::Down, sf::Keyboard::Down);
	setKey(Input::Up, sf::Keyboard::Up);
	setKey(Input::Action, sf::Keyboard::Z);
	setKey(Input::Dialogue, sf::Keyboard::X);
	setKey(Input::Start, sf::Keyboard::Return);
	setKey(Input::Exit, sf::Keyboard::Escape);
}


void InputHandler::setKey(Input id, sf::Keyboard::Key keyCode)
{
	auto &key = inputs[id];
	key.inputType = Key;
	key.eventType = sf::Event::KeyPressed;
	key.keyCode = keyCode;
}


/* Actions */
bool InputHandler::checkInput(Input i, sf::Event e)
{
	auto &input = inputs[i];
	// Mouse event
	if (input.inputType == MouseInput &&
		input.eventType == e.type &&
		input.mouseButton == e.mouseButton.button) return true;
	// KeyPress event
	else if (input.inputType == Key &&
		input.eventType == e.type &&
		input.keyCode == e.key.code) return true;
	else return false;
}

bool InputHandler::checkInput(Input i)
{
	auto &input = inputs[i];
	// KeyHeld event
	if (input.inputType == Key &&
		sf::Keyboard::isKeyPressed(input.keyCode)) return true;
	else return false;
}
