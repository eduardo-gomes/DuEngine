#include <string>

//Function to start and stop

//Create Window with title WindowName and enter MainLoop
//if fail return false
bool Start(const std::string &WindowName);
//Exit mainLoop
void Stop();

//External init
extern void Setup();
