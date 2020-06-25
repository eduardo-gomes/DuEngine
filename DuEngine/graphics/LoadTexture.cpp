#include "LoadTexture.hpp"

#include <fstream>
#include <iostream>

unsigned char *LoadTexture(const std::string &FilePath, int *Width, int *Height) {
	std::ifstream file(FilePath, std::ios::binary | std::ios::ate);
	file.seekg(0, std::ios::beg);
	char *fdata;
	if (!file.good()) {
		std::cout << FilePath << " !good" << std::endl;
		return nullptr;
	}
	char header[54];
	file.read(header, 54);
	unsigned int width = *(unsigned int *)&header[18], height = *(unsigned int *)&header[22], offset = *(unsigned int *)&header[10];
	*Width = (int)width;
	*Height = (int)height;
	const int color_depth = 4;
	int datasize = (int)(width * height * color_depth);
	fdata = (char *)malloc(width * height * color_depth);
	if (fdata == NULL) {
		std::cout << "can't malloc to load file " << FilePath << std::endl;
		return nullptr;
	}
	file.seekg(offset, std::ios::beg);
	if (!file.fail()) {
		file.read(fdata, datasize);
		if (file.fail()) {
			std::cout << FilePath << " file load fail" << std::endl;
			free(fdata);
			return nullptr;
		}
		/*//undo upside down
		uint32_t a[width], b[width], *start = (uint32_t *)fdata;
		unsigned int line_len = width * sizeof(uint32_t);
		for(unsigned line = 0; line < height/2; line++){
			unsigned linea = line, lineb = height - line - 1;//linea top, lineb bottom
			memcpy(a, start + (linea * width) * line_len, line_len);//top to a
			memcpy(b, start + (lineb * width) * line_len, line_len);//bottom to b

			memcpy(start + (lineb * width) * line_len, a, line_len);//a to bottom
			memcpy(start + (linea * width) * line_len, b,line_len);//b to top
		}*/

		//convert to RGBA
		uint32_t *image = (uint32_t *)fdata;
		//std::cout << (int)fdata[6*4] << " " << (int)fdata[6*4+1] << " " << (int)fdata[6*4+2] << " " << (int)fdata[6*4+3]<< " " << std::endl;
		for (unsigned pixel = 0; pixel < width * height; ++pixel) {
			// from 0xBBGGRRAA to 0xRRGGBBAA
			// but litle endian so
			// from 0xAARRGGBB to 0XAABBGGRR
			image[pixel] =
				(image[pixel] & 0xFF00FF00) |		 //AA__GG__
				(image[pixel] & 0x000000FF) << 16 |	 //__BB____
				(image[pixel] & 0x00FF0000) >> 16;	 //______RR
													 //0xAABBGGRR
		}
		return (unsigned char *)fdata;
	}
	free(fdata);
	std::cout << "Failed to read " << FilePath << std::endl;
	return nullptr;
}
