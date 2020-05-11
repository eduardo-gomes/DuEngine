#pragma once
#include <fstream>
#include <string>
#include <future>
#include <vector>
#include <mutex>
#ifndef _MAIN
#include "colision.cpp"
#endif

GLuint textures[assets_size];

int loadtexture(long unsigned index, std::array<std::string, assets_size> *assetsToLoad_ptr, void *saveto[], int size[][2]){
	std::array<std::string, assets_size> &assetsToLoad = *assetsToLoad_ptr;
	std::ifstream file(assetsToLoad[index], std::ios::binary | std::ios::ate);
	file.seekg(0, std::ios::beg);
	char* fdata;
	if(!file.good()){
		//std::cout << assetsToLoad[index] << " !good" << std::endl;
		return 3;
	}
	char header[54];
	file.read(header, 54);
	unsigned int width = *(unsigned int*)&header[18], height = *(unsigned int*)&header[22], offset = *(unsigned int*)&header[10];
	size[index][0] = (int)width;
	size[index][1] = (int)height;
	const int color_depth = 4;
	int datasize = (int)(width * height * color_depth);
	saveto[index] = fdata = (char*)malloc(width * height * color_depth);
	if(fdata == NULL){
		//std::cout << "can't malloc to load file" << std::endl;
		return 2;
	}
	file.seekg(offset, std::ios::beg);
	if(!file.fail()){
		file.read(fdata, datasize);
		if(file.fail()){
			//std::cout << assetsToLoad[index] << " load fail" << std::endl;
			return 1;
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
			return 0;
		}
	}
	return 1;
}

int loadassets(){
	glGenTextures(assets_size, textures);
	std::array<std::future<int>, assets_size> futures;
	void* images[sprites_end];
	int imagesize[sprites_end][2];//w h
	std::array<std::string, assets_size> &assetsToLoad = *get_assets_files();
	for (long unsigned i = sprites_begin; i < sprites_end; ++i) {
		futures[i] = std::async(loadtexture, i, &assetsToLoad, images, imagesize);
	}
	for (long unsigned i = sprites_begin; i < sprites_end; ++i) {
		if(futures[i].get())
			std::cout << "Can't load " << assetsToLoad[i] << std::endl;
		glBindTexture(GL_TEXTURE_2D, textures[i]);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, imagesize[i][0], imagesize[i][1], 0, GL_RGBA, GL_UNSIGNED_BYTE, images[i]);//the bitmap file is bottom-top, so the texture origin is on the bottom left corner instead of being on the top left
		free(images[i]);
	}
	/*for (long unsigned i = sounds_begin; i < sounds_end; ++i) {
		if(futures[i].get())
			std::cout << "Can't load " << assetsToLoad[i] << std::endl;
	}*/
	//std::cout << "load end" << std::endl;
	delete &assetsToLoad;
	return 0;
}
