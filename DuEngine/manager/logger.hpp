#include <string>
#include <DuEngine/visibility.hpp>
namespace logger {
void DUENGINT dbug(const std::string& msg);
void DUENGINT info(const std::string& msg);
void DUENGINT warn(const std::string& msg);
void DUENGINT erro(const std::string& msg);
void DUENGINT excp(const std::string& msg);
}  // namespace log