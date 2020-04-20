#pragma once
#include <fstream>
#include <array>
#include <string>
#include <future>
#include <vector>
#ifndef _MAIN
#include "demo.cpp"
#endif
const long unsigned assets_size = 8;
GLuint textures[assets_size];
std::array<std::string, assets_size> assetsToLoad;

void loadtexture(long unsigned index){
	std::ifstream file(assetsToLoad[index], std::ios::binary | std::ios::ate);
	file.seekg(0, std::ios::beg);
	char* fdata;
	if(!file.good())
		std::cout << assetsToLoad[index] << " !good" << std::endl;
	char header[54];
	file.read(header, 54);
	unsigned int width = *(unsigned int*)&header[18], height = *(unsigned int*)&header[22], offset = *(unsigned int*)&header[10];
	const int color_depth = 4;
	int datasize = (int)(width * height * color_depth);
	fdata = (char*)malloc(width * height * color_depth);
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
			//std::cout << (int)fdata[6*4] << " " << (int)fdata[6*4+1] << " " << (int)fdata[6*4+2] << " " << (int)(unsigned char)fdata[6*4+3]<< " " << std::endl;
			//std::cout << "w: " << width << " H: " << height << " OF: " << offset << std::endl;
			glBindTexture(GL_TEXTURE_2D, textures[index]);
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, (int)width, (int)height, 0, GL_RGBA, GL_UNSIGNED_BYTE, fdata);
			//if(index == 1) std::cout << "loaded " << index << " to " << textures << std::endl;
		}
	}
	free(fdata);
}

int loadassets(){
	assetsToLoad[0] = "assets/pers01.bmp";
	assetsToLoad[1] = "assets/pers02.bmp";
	assetsToLoad[2] = "assets/pers03.bmp";
	assetsToLoad[3] = "assets/pers04.bmp";
	assetsToLoad[4] = "assets/pers05.bmp";
	assetsToLoad[5] = "assets/pers06.bmp";
	assetsToLoad[6] = "assets/pers07.bmp";
	assetsToLoad[7] = "assets/pers09.bmp";
	glGenTextures(assets_size, textures);
	for (long unsigned i = 0; i < assets_size; ++i) {
		loadtexture(i);
	}
	std::cout << "load end" << std::endl;
	return 0;
}