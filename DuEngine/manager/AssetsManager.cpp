#include "AssetsManager.hpp"

#include <chrono>

#include "graphics/GLClasses.hpp"

namespace Manager {
class Manager {
	/*enum AssetType { BITMAP,
					 OGG,
					 SAVEFILE };
	std::map<std::string, asset> AssetsMap;*/
	class asset {
		const std::string FilePath;
		unsigned const Type;
		asset() = delete;
	};
	void LoadSong(std::string FilePath, std::string Name);
	//Texture& GetSong(std::string Name);

	std::map<std::string, asset> AssetsMap;
	std::map<std::string, Texture> TexturesMap;
};
}  // namespace Manager

Manager::log::log() {
	std::string filename("log-");
	std::time_t t = std::time(nullptr);
	std::tm time = *std::localtime(&t);
	filename += std::to_string(time.tm_year + 1900) + "-"
			  + std::to_string(time.tm_mon + 1) + "-"
			  + std::to_string(time.tm_mday) + "-"
			  + std::to_string(time.tm_hour) + "-"
			  + std::to_string(time.tm_min) + "-"
			  + std::to_string(time.tm_sec) + ".txt";
	logFile.open(filename, (std::fstream::out | std::fstream::app));
}
Manager::log::~log() {
	logFile.close();
}
inline int Manager::log::writeToFile(const std::string &toWrite) {
	logFile << toWrite << std::endl;
	return 0;
}
int Manager::log::write(const std::string &toWrite) {
	if (logger == nullptr) logger = new log();
	return logger->write(toWrite);
}
