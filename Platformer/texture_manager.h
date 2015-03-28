#ifndef TEXTURE_MANAGER_H
#define TEXTURE_MANAGER_H

#include <SFML/Graphics.hpp>
#include <iostream>
#include <string>
#include <map>
#include "globals.h"

class TextureManager
{
	private:
		std::map<std::string, sf::Texture> textures;
	public:
		TextureManager();
		~TextureManager();

		// Actions
		void loadTexture(const std::string &name, const std::string &filename);
		sf::Texture const& getRef(const std::string &texture);
};

#endif
