#include "logger.hpp"

#include <chrono>
#include <ctime>
#include <iostream>

#include "AssetsManager.hpp"
namespace logger {

enum type { EXCP,
			ERRO,
			WARN,
			INFO,
			DEBUG };
void logger(const std::string& msg, int type) {
	std::string log;
	std::time_t t = std::time(nullptr);
	std::tm time = *std::localtime(&t);
	char timestr[80];
	const char* COLOR_STR[4] = {"\x1b[0m", "\x1b[33m", "\x1b[31m", "\x1b[36m"};
	enum COLOR_INDEX{DEFAULT, RED, YELLOW, CYAN};
	int color;

	std::strftime(timestr, 80, "[%F %T %Z] ", &time);
	log += timestr;
	switch (type) {
		case EXCP:
			log += "Exception: ";
			color = RED;
			break;
		case ERRO:
			log += "Error: ";
			color = RED;
			break;
		case WARN:
			log += "Warning: ";
			color = YELLOW;
			break;
		case INFO:
			log += "Info: ";
			color = DEFAULT;
			break;
		case DEBUG:
			log += "DEBUG: ";
			color = CYAN;
			break;
		default:
			log += "Log: ";
			color = DEFAULT;
			break;
	}
	printf("%s%s%s%s\n", log.c_str(), COLOR_STR[color], msg.c_str(), COLOR_STR[DEFAULT]);
	log += msg;
	Man::log::write(log);
}
void dbug(const std::string& msg) {
	logger(msg, DEBUG);
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

}  // namespace logger