#include "texture_manager.h"
#include "globals.h"

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

#ifdef _DEBUG
	std::cout << "Loaded texture \"" << name << "\" from " << filename << std::endl;
#endif

	/* Add it to the list of textures */
	this->textures[name] = tex;

	return;
}

sf::Texture const& TextureManager::getRef(const std::string &texture)
{
	static sf::Texture const placeholder;

	if (textures.find(texture) != textures.end()) return this->textures.at(texture);
	else return placeholder;
}
