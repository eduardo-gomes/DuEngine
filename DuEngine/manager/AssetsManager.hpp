#include <fstream>
#include <list>
#include <map>
#include <mutex>
#include <string>
#include <thread>

#include <DuEngine/audio/audio.hpp>
#include "logger.hpp"

namespace Man {
class DUENGEXT Manager {
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
	void CloseOggLoaderThread();
	std::thread OggLoaderThread;
	bool OggLoaderContinue;
	std::mutex list, map;
	std::list<std::pair<std::unique_ptr<audio::converter>, std::unique_ptr<audio::ogg_read>>> loadingOgg;
	std::map<std::string, std::shared_ptr<audio::WAVE>> waves;
	void OggLoader();
	friend audio::audio::~audio();

   public:
	Manager();
	~Manager();
	const std::shared_ptr<audio::WAVE>& LoadOGG(std::string filePath);
	static std::unique_ptr<Manager> Instance;
	//audio::WAVE& GetOGG(std::string filePath) const;
};
class DUENGINT log {
	std::fstream logFile;
	static log* logger;
	log();
	~log();

   public:
	int writeToFile(const std::string& log);
	static int write(const std::string& log);
	static void close();
};
}  // namespace Man