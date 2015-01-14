#include "texture_manager.h"

TextureManager::TextureManager()
{
	
}

TextureManager::~TextureManager()
{
	textures.clear();
}


/* Actions */
void TextureManager::loadTexture(const std::string &name, const std::string &filename)
{
	/* Load the texture */
	sf::Texture tex;
	tex.loadFromFile(filename);

	/* Add it to the list of textures */
	this->textures[name] = tex;

	return;
}

sf::Texture& TextureManager::getRef(const std::string &texture)
{
	if (textures.find(texture) != textures.end()) return this->textures.at(texture);
	else return sf::Texture();
}
