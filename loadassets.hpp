#pragma once
#include <fstream>
#include <array>
#include <string>
#include <future>
#include <vector>
#ifndef _MAIN
#include "demo.cpp"
#endif
const long unsigned assets_size = 9;
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

enum sprites{pers01, pers02, pers03, pers04, pers05, pers06, pers07, pers09, brick};
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
	glGenTextures(assets_size, textures);
	for (long unsigned i = 0; i < assets_size; ++i) {
		loadtexture(i);
	}
	std::cout << "load end" << std::endl;
	return 0;
}

struct vector_quad_text{
	double vector[16] = {
		//pos		text pos
		0.0, 0.0,	0.0, 0.0,
		0.0, 0.0,	0.0, 0.0,
		0.0, 0.0,	0.0, 0.0,
		0.0, 0.0,	0.0, 0.0
	};
	int text_index;
};
void drawn_quad_with_texture(const vector_quad_text& vector){
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, textures[vector.text_index]);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);//repeat texture on x DISABLED because is the defaut
	//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);//repeat texture on y
	glColor3d(1.0, 1.0, 1.0);
	glBegin(GL_QUADS);
		glTexCoord2d(vector.vector[2] , vector.vector[3] );glVertex2d(vector.vector[0] , vector.vector[1] );
		glTexCoord2d(vector.vector[6] , vector.vector[7] );glVertex2d(vector.vector[4] , vector.vector[5] );
		glTexCoord2d(vector.vector[10], vector.vector[11]);glVertex2d(vector.vector[8] , vector.vector[9] );
		glTexCoord2d(vector.vector[14], vector.vector[15]);glVertex2d(vector.vector[12], vector.vector[13]);
	glEnd();
	glDisable(GL_TEXTURE_2D);
}