#ifndef INPUT_HANDLER_H
#define INPUT_HANDLER_H

#include <SFML/Graphics.hpp>
#include <string>
#include <map>

class InputHandler
{
	private:
		enum InputType
		{
			KeyPress,
			KeyHeld,
			MouseInput,
			JoystickInput
		};
		struct InputKeys
		{
			InputType inputType;
			sf::Event::EventType eventType;
			sf::Keyboard::Key keyCode;
			sf::Mouse::Button mouseButton;
		};

		std::map<std::string, InputKeys*> Keys;
	public:
		InputHandler();

		// Actions
		bool checkInput(std::string k, sf::Event e);
		bool checkInput(std::string k);
};

#endif
