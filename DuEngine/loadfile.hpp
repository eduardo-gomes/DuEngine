#include <string>

namespace audio {struct sound;}

unsigned char *LoadTexture(const std::string &FilePath, int *Width, int *Height);

//audio::sound &LoadSound(const std::string &FilePath);