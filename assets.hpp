#pragma once
#include <array>
#include <string>

//std::array<std::string, assets_size> assetsToLoad;

enum sprites{pers01, pers02, pers03, pers04, pers05, pers06, pers07, pers09, brick, rgba, funuv, nuvem, coque, coin, help};//help is the last
enum sounds{scoin = help +1};
enum spritesname{
	persparado00 = pers01,
	persparado01 = pers02,
	persandando01 = pers03,
	persandando02 = pers04,
	persandando03 = pers05,
	persandando04 = pers06,
	persparado02 = pers07,
	perspulando01 = pers09
};

constexpr long unsigned sprites_begin =0;
constexpr long unsigned sprites_end = sprites::help + 1;
constexpr long unsigned sounds_begin = sounds::scoin;
constexpr long unsigned sounds_end = sounds::scoin + 1;
constexpr long unsigned assets_size = sprites_end;//sounds_end;// until implement function to load sound
std::array<std::string, assets_size>* get_assets_files(){
	std::array<std::string, assets_size> & assetsToLoad = *(new std::array<std::string, assets_size>);
	assetsToLoad[pers01] = "assets/pers01.bmp";
	assetsToLoad[pers02] = "assets/pers02.bmp";
	assetsToLoad[pers03] = "assets/pers03.bmp";
	assetsToLoad[pers04] = "assets/pers04.bmp";
	assetsToLoad[pers05] = "assets/pers05.bmp";
	assetsToLoad[pers06] = "assets/pers06.bmp";
	assetsToLoad[pers07] = "assets/pers07.bmp";
	assetsToLoad[pers09] = "assets/pers09.bmp";
	assetsToLoad[brick] = "assets/brick.bmp";
	assetsToLoad[rgba] = "assets/rgb.bmp";
	assetsToLoad[funuv] = "assets/funuv.bmp";
	assetsToLoad[nuvem] = "assets/nuvem.bmp";
	assetsToLoad[coque] = "assets/coque.bmp";
	assetsToLoad[help] = "assets/help1024x303.bmp";
	assetsToLoad[coin] = "assets/coin.bmp";
	//assetsToLoad[scoin] = "assets/sfx_coin_double1.wav";
	return &assetsToLoad;
}