#pragma once
#include <ctime>
#include <iomanip>

#include "Render.h"
#include "Model.h"

class SimpleGameEngine {
private:
    HINSTANCE  hInstance;
    Render*    pRender;
    Model      mCube;
    double     dTime;
    
    double     epochTime;
    int        framesRendered;
    
    static LRESULT CALLBACK wndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);

public:
    SimpleGameEngine(HINSTANCE hInstance, int width, int height);
    Render* getRender();
    int runWindowsMessageLoop();
    void update();
};