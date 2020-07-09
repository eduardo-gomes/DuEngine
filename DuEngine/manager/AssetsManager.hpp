#include <fstream>
#include <list>
#include <map>
#include <mutex>
#include <string>
#include <thread>

#include "../audio/audio.hpp"
#include "logger.hpp"

namespace Manager {
class Manager {
	enum AssetType { BITMAP,
					 OGG,
					 SAVEFILE };
	class asset {
		const std::string FilePath;
		unsigned const Type;
		void* ptr;
		asset() = delete;
		virtual ~asset();
	};
	void SpawnOggLoaderThread();
	std::thread OggLoaderThread;
	bool OggLoaderContinue;
	std::mutex list, map;
	std::list<std::pair<std::unique_ptr<audio::converter>, std::unique_ptr<audio::ogg_read>>> loadingOgg;
	std::map<std::string, std::shared_ptr<audio::WAVE>> waves;
	void OggLoader();

   public:
	Manager();
	~Manager();
	const std::shared_ptr<audio::WAVE>& LoadOGG(std::string filePath);
	//audio::WAVE& GetOGG(std::string filePath) const;
};
extern std::unique_ptr<Manager> Insatance;
class log {
	std::fstream logFile;
	static log* logger;
	log();
	~log();

   public:
	int writeToFile(const std::string& log);
	static int write(const std::string& log);
	static void close();
};
}  // namespace Manager