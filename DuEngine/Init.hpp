#include <string>
#include <DuEngine/visibility.hpp>

//Function to start and stop

//Create Window with title WindowName and enter MainLoop
//if fail return false
bool DUENGEXT Start(const std::string &WindowName, int AUDIO = 0);
//Exit mainLoop
void DUENGEXT Stop();

//External init callback run after window creation and before enter mainloop
extern DUENGEXT void (*Setup)();
