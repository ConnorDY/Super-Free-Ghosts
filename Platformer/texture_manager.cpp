#include "texture_manager.h"
#include "globals.h"

TextureManager::TextureManager()
{
}

TextureManager::~TextureManager()
{
}


/* Actions */
void TextureManager::loadTexture(const std::string &name, const std::string &filename)
{
	/* Load the texture */
	this->textures[name].loadFromFile(filename);

#ifdef _DEBUG
	std::cout << "Loaded texture \"" << name << "\" from " << filename << std::endl;
#endif

}

void TextureManager::loadTextureFromMemory(const std::string &name, unsigned char const *memory, size_t size)
{
	this->textures[name].loadFromMemory(memory, size);
#ifdef _DEBUG
	std::cout << "Loaded texture \"" << name << "\"" << std::endl;
#endif
}

sf::Texture const& TextureManager::getRef(const std::string &texture)
{
	static sf::Texture const placeholder;

	if (textures.find(texture) != textures.end()) return this->textures.at(texture);
	else return placeholder;
}
