#include <string>
namespace log {
void info(const std::string& msg);
void warn(const std::string& msg);
void erro(const std::string& msg);
void excp(const std::string& msg);
}  // namespace log