#pragma once
#include <ctime>
#include <iomanip>

#include "Draw.h"
#include "Render.h"
#include "Model.h"

class SimpleGameEngine {
private:
    HINSTANCE  hInstance;
    Draw*      pDraw;
    Render*    pRender;
    Model      mCube;
    double     dTime;
    
    double     epochTime;
    int        framesRendered;
    
    static LRESULT CALLBACK wndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);

public:
    SimpleGameEngine(HINSTANCE hInstance, int width, int height);
    Draw* getDraw();
    int runWindowsMessageLoop();
    void draw();
};