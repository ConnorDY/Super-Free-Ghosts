#include <SFML/Graphics.hpp>
#include <vector>

#include "globals.h"
#include "settings.h"
#include "texture_manager.h"
#include "sound_manager.h"
#include "state_manager.h"

#ifdef STATIC_RESOURCES
#include "static_resource.h"
#endif

/* Load Game's Resources */
void loadRes(TextureManager &textureManager, SoundManager &soundManager)
{
	// TODO:
	// This is a clusterfuck, we can probably do this better.
	// Either we generate an include file that does this (we'll need a text file for mapping name to file in that case)
	// Or we use macros to expand to the right one e.g. RES("player-dead", res_plyr_plyr_dead_png, "res/plyr/plyr_dead.png")
	// Keeping this for now though because there's no real reason to work on it so close to the deadline for demo
#ifdef STATIC_RESOURCES
	auto textures = std::vector<std::tuple<std::string, unsigned char const*, size_t>>
	{
		std::make_tuple<std::string, unsigned char const*, size_t>("player-dead", ARRAY_WITH_LENGTH(res_plyr_plyr_dead_png)),
		std::make_tuple<std::string, unsigned char const*, size_t>("player-naked", ARRAY_WITH_LENGTH(res_plyr_plyr_naked_png)),
		std::make_tuple<std::string, unsigned char const*, size_t>("player-silver", ARRAY_WITH_LENGTH(res_plyr_plyr_silver_png)),
		std::make_tuple<std::string, unsigned char const*, size_t>("player-gold", ARRAY_WITH_LENGTH(res_plyr_plyr_gold_png)),
		std::make_tuple<std::string, unsigned char const*, size_t>("transform-silver", ARRAY_WITH_LENGTH(res_plyr_transform_silver_png)),
		std::make_tuple<std::string, unsigned char const*, size_t>("transform-gold", ARRAY_WITH_LENGTH(res_plyr_transform_gold_png)),
		std::make_tuple<std::string, unsigned char const*, size_t>("chest1", ARRAY_WITH_LENGTH(res_chest_01_png)),
		std::make_tuple<std::string, unsigned char const*, size_t>("zombie", ARRAY_WITH_LENGTH(res_zombie_png)),
		std::make_tuple<std::string, unsigned char const*, size_t>("tiles", ARRAY_WITH_LENGTH(res_tiles_png)),
		std::make_tuple<std::string, unsigned char const*, size_t>("obelisk", ARRAY_WITH_LENGTH(res_obelisk_png)),
		std::make_tuple<std::string, unsigned char const*, size_t>("bg01", ARRAY_WITH_LENGTH(res_bg01_png)),
		std::make_tuple<std::string, unsigned char const*, size_t>("bg02", ARRAY_WITH_LENGTH(res_bg02_png)),
		std::make_tuple<std::string, unsigned char const*, size_t>("bg03", ARRAY_WITH_LENGTH(res_bg03_png)),
		std::make_tuple<std::string, unsigned char const*, size_t>("under01", ARRAY_WITH_LENGTH(res_under01_png)),
		std::make_tuple<std::string, unsigned char const*, size_t>("logo", ARRAY_WITH_LENGTH(res_logo_png)),
		std::make_tuple<std::string, unsigned char const*, size_t>("spear", ARRAY_WITH_LENGTH(res_weapons_spear_png)),
		std::make_tuple<std::string, unsigned char const*, size_t>("hammer", ARRAY_WITH_LENGTH(res_weapons_hammer_png)),
		std::make_tuple<std::string, unsigned char const*, size_t>("trident", ARRAY_WITH_LENGTH(res_weapons_trident_png)),
		std::make_tuple<std::string, unsigned char const*, size_t>("melee", ARRAY_WITH_LENGTH(res_weapons_melee_png)),
		std::make_tuple<std::string, unsigned char const*, size_t>("handeye", ARRAY_WITH_LENGTH(res_enemy_handeye_png)),
		std::make_tuple<std::string, unsigned char const*, size_t>("enemydie", ARRAY_WITH_LENGTH(res_enemy_explosion_png)),
		std::make_tuple<std::string, unsigned char const*, size_t>("weapon_drops", ARRAY_WITH_LENGTH(res_weapons_idle_png)),
	};

	auto sounds = std::vector<std::tuple<std::string, unsigned char const*, size_t>>
	{
		std::make_tuple<std::string, unsigned char const*, size_t>("jump", ARRAY_WITH_LENGTH(res_jump_wav)),
		std::make_tuple<std::string, unsigned char const*, size_t>("land", ARRAY_WITH_LENGTH(res_land_wav)),
		std::make_tuple<std::string, unsigned char const*, size_t>("throw", ARRAY_WITH_LENGTH(res_throw_wav)),
		std::make_tuple<std::string, unsigned char const*, size_t>("hit", ARRAY_WITH_LENGTH(res_hit_wav)),
		std::make_tuple<std::string, unsigned char const*, size_t>("enemy_die", ARRAY_WITH_LENGTH(res_enemy_die_wav)),
	};

	for (auto &texture : textures) textureManager.loadTextureFromMemory(std::get<0>(texture), std::get<1>(texture), std::get<2>(texture));
	for (auto &sound : sounds) soundManager.loadSoundFromMemory(std::get<0>(sound), std::get<1>(sound), std::get<2>(sound));
#else
	auto textures = std::vector<std::pair<std::string, std::string>>
	{
		{ "player-dead", "res/plyr/plyr_dead.png" },
		{ "player-naked", "res/plyr/plyr_naked.png" },
		{ "player-silver", "res/plyr/plyr_silver.png" },
		{ "player-gold", "res/plyr/plyr_gold.png" },
		{ "transform-silver", "res/plyr/transform_silver.png" },
		{ "transform-gold", "res/plyr/transform_gold.png" },
		{ "chest1", "res/chest/01.png" },
		{ "zombie", "res/zombie.png" },
		{ "tiles", "res/tiles.png" },
		{ "obelisk", "res/obelisk.png" },
		{ "bg01", "res/bg01.png" },
		{ "bg02", "res/bg02.png" },
		{ "bg03", "res/bg03.png" },
		{ "under01", "res/under01.png" },
		{ "logo", "res/logo.png" },
		{ "spear", "res/weapons/spear.png" },
		{ "hammer", "res/weapons/hammer.png" },
		{ "trident", "res/weapons/trident.png" },
		{ "melee", "res/weapons/melee.png" },
		{ "handeye", "res/enemy/handeye.png" },
		{ "enemydie", "res/enemy/explosion.png" },
		{ "weapon_drops", "res/weapons/idle.png" },
	};

	auto sounds = std::vector<std::pair<std::string, std::string>>
	{
		{ "jump", "res/jump.wav" },
		{ "land", "res/land.wav" },
		{ "throw", "res/throw.wav" },
		{ "hit", "res/hit.wav" },
		{ "enemy_die", "res/enemy_die.wav" },
	};

	for (auto &texture : textures) textureManager.loadTexture(texture.first, texture.second);
	for (auto &sound : sounds) soundManager.loadSound(sound.first, sound.second);
#endif
}


/* Program Entry Point */
int main()
{
	// Default settings
	settings_t settings;
	settings.window_scale = 2;
	settings.vsync_on = true;
	settings.music_on = true;
	settings.sound_on = true;

	// Load Textures and Sounds and Shaders
	TextureManager textureManager;
	SoundManager soundManager(settings);
	loadRes(textureManager, soundManager);

	// Create State Manager
	StateManager stateManager(textureManager, soundManager, settings);
	
	// Create Window
	sf::RenderWindow window(sf::VideoMode(VIEW_WIDTH * settings.window_scale, VIEW_HEIGHT * settings.window_scale), "Super Free Ghosts", sf::Style::Close | sf::Style::Titlebar);
	window.setVerticalSyncEnabled(settings.vsync_on);

	// Game Loop
	while (window.isOpen())
	{
		// Update State
		stateManager.update(window);

		// Draw the State
		window.clear();
		stateManager.draw(window);
		window.display();
	}

	// Exit
	return 0;
}
