#pragma once
#include <fstream>
#include <array>
#include <string>
#include <future>
#include <vector>
#include <mutex>
#ifndef _MAIN
#include "colision.cpp"
#endif
const long unsigned assets_size = 10;
GLuint textures[assets_size];
std::array<std::string, assets_size> assetsToLoad;

void loadtexture(long unsigned index, void* saveto[], int size[][2]) {
	std::ifstream file(assetsToLoad[index], std::ios::binary | std::ios::ate);
	file.seekg(0, std::ios::beg);
	char* fdata;
	if(!file.good())
		std::cout << assetsToLoad[index] << " !good" << std::endl;
	char header[54];
	file.read(header, 54);
	unsigned int width = *(unsigned int*)&header[18], height = *(unsigned int*)&header[22], offset = *(unsigned int*)&header[10];
	size[index][0] = (int)width;
	size[index][1] = (int)height;
	const int color_depth = 4;
	int datasize = (int)(width * height * color_depth);
	saveto[index] = fdata = (char*)malloc(width * height * color_depth);
	if(fdata == NULL){
		std::cout << "can't malloc" << std::endl;
		exit(0);
	}
	file.seekg(offset, std::ios::beg);
	if(!file.fail()){
		file.read(fdata, datasize);
		if(file.fail()){
			std::cout << assetsToLoad[index] << " load fail" << std::endl;
			exit(0);
		}else{
			//convert to RGBA
			uint32_t* image = (uint32_t*)fdata;
			//std::cout << (int)fdata[6*4] << " " << (int)fdata[6*4+1] << " " << (int)fdata[6*4+2] << " " << (int)fdata[6*4+3]<< " " << std::endl;
			for(unsigned pixel = 0; pixel < width * height; ++pixel){
				// from 0xBBGGRRAA to 0xRRGGBBAA 
				// but litle endian so
				// from 0xAARRGGBB to 0XAABBGGRR
				image[pixel] =
				(image[pixel] & 0xFF00FF00)		  | //AA__GG__
				(image[pixel] & 0x000000FF) << 16 | //__BB____
				(image[pixel] & 0x00FF0000) >> 16  ; //______RR
												  //0xAABBGGRR
			}
		}
	}
}

enum sprites{pers01, pers02, pers03, pers04, pers05, pers06, pers07, pers09, brick, rgba};
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

int loadassets(){
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
	glGenTextures(assets_size, textures);
	std::array<std::future<void>, assets_size> futures;
	void* images[assets_size];
	int imagesize[assets_size][2];//w h
	for (long unsigned i = 0; i < assets_size; ++i) {
		futures[i] = std::async(loadtexture, i, images, imagesize);
	}
	for (long unsigned i = 0; i < assets_size; ++i) {
		futures[i].wait();
		glBindTexture(GL_TEXTURE_2D, textures[i]);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, imagesize[i][0], imagesize[i][1], 0, GL_RGBA, GL_UNSIGNED_BYTE, images[i]);
		free(images[i]);
	}
	//std::cout << "load end" << std::endl;
	return 0;
}
