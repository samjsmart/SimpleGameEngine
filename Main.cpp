#include "Main.h"

INT WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PSTR lpCmdLine, INT nCmdShow) {    
    SimpleGameEngine gameEngine(hInstance, 1920, 1080);

    return gameEngine.runWindowsMessageLoop();
}