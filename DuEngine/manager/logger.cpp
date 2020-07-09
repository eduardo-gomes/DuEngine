#include "logger.hpp"

#include <chrono>
#include <ctime>
#include <iostream>

#include "AssetsManager.hpp"
namespace logger {

enum type { EXCP,
			ERRO,
			WARN,
			INFO };
void logger(const std::string& msg, int type) {
	std::string log;
	std::time_t t = std::time(nullptr);
	std::tm time = *std::localtime(&t);
	char timestr[80];

	std::strftime(timestr, 80, "[%F %T %Z] ", &time);
	log += timestr;
	switch (type){
	case EXCP:
		log += "Exception: ";
		break;
	case ERRO:
		log += "Error: ";
		break;
	case WARN:
		log += "Warning: ";
		break;
	case INFO:
		log += "Info: ";
		break;
	default:
		log += "Log: ";
		break;
	}
	log += msg;

	Manager::log::write(log);
	std::cout << log << std::endl;
}
void info(const std::string& msg) {
	logger(msg, INFO);
}
void warn(const std::string& msg) {
	logger(msg, WARN);
}
void erro(const std::string& msg) {
	logger(msg, ERRO);
}
void excp(const std::string& msg) {
	logger(msg, EXCP);
}

}  // namespace log