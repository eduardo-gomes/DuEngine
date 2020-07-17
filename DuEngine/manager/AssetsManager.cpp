#include "AssetsManager.hpp"

#include <chrono>

#include "graphics/GLClasses.hpp"
namespace Man {
std::unique_ptr<Manager> Man::Manager::Insatance;
}
Man::Manager::Manager() : OggLoaderContinue(1) {
}
Man::Manager::~Manager() {
	OggLoaderContinue = false;
	logger::info("Waiting OggLoader Thread to finish");
	if (OggLoaderThread.joinable()) OggLoaderThread.join();
	logger::info("OggLoader Thread to finished");
}
inline void Man::Manager::SpawnOggLoaderThread() {
	OggLoaderContinue = true;
	if (!OggLoaderThread.joinable())
		OggLoaderThread = std::thread(&Manager::OggLoader, this);
}
void Man::Manager::OggLoader() {
	while (OggLoaderContinue) {
		if (!loadingOgg.size()) {
			std::this_thread::sleep_for(std::chrono::milliseconds(250));
			continue;
		}
		std::lock_guard<std::mutex> lg(list);
		for (auto it = loadingOgg.begin(); it != loadingOgg.end(); ++it) {
			audio::ogg_read &read = *it->second;
			audio::converter &conv = *it->first;
			char buffer[4096];
			int readRet = read.read(buffer, sizeof(buffer));
			if (readRet == 0) {	 //EOF
				conv.flush();
				it = loadingOgg.erase(it);
				if (it == loadingOgg.end())
					break;
				else
					--it;
				continue;
			} else if (readRet < 0) {
				logger::erro("OggLoader readRet < 0");
				continue;
			}
			conv.put(buffer, static_cast<size_t>(readRet));
		}
	}
}
const std::shared_ptr<audio::WAVE> &Man::Manager::LoadOGG(std::string filePath) {
	std::lock_guard<std::mutex> lg(map);
	auto find = waves.find(filePath);
	if (find != waves.end()) {
		return find->second;
	} else {
		std::ifstream file(filePath);
		if (!file.good()) throw std::runtime_error("Cant open file: " + filePath);
		auto read = std::make_unique<audio::ogg_read>(std::move(file));
		auto from = read->getSpec(), to = audio::audioOut->getSpec();
		auto conv = std::make_unique<audio::converter>(from, read->samples, to);
		{
			std::lock_guard<std::mutex> lg(list);
			waves.emplace(filePath, conv->getWAVE());
			loadingOgg.emplace_back(std::make_pair(std::move(conv), std::move(read)));
		}
		SpawnOggLoaderThread();
		return waves[filePath];
	}
}

Man::log::log() {
	std::string filename("log-");
	std::time_t t = std::time(nullptr);
	std::tm time = *std::localtime(&t);
	filename += std::to_string(time.tm_year + 1900) + "-" + std::to_string(time.tm_mon + 1) + "-" + std::to_string(time.tm_mday) + "-" + std::to_string(time.tm_hour) + "-" + std::to_string(time.tm_min) + "-" + std::to_string(time.tm_sec) + ".txt";
	logFile.open(filename, (std::fstream::out | std::fstream::app));
	printf("Opened Log\n");
}
Man::log *Man::log::logger = nullptr;
Man::log::~log() {
	logger::info("Closing Log");
	logFile.close();
	//printf("Closed Log\n");
}
inline int Man::log::writeToFile(const std::string &toWrite) {
	logFile << toWrite << std::endl;
	return 0;
}
int Man::log::write(const std::string &toWrite) {
	if (logger == nullptr) logger = new log();
	return logger->writeToFile(toWrite);
}
void Man::log::close() {
	if (logger != nullptr) delete logger;
	logger = nullptr;
}
