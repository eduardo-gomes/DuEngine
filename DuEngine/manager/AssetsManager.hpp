#include <fstream>
#include <map>
#include <string>

namespace Manager {
class Manager;
class log {
	std::fstream logFile;
	static log* logger;
	log();

   public:
	~log();
	int writeToFile(const std::string& log);
	static int write(const std::string& log);
};
}  // namespace Manager