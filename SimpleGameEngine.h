#pragma once
#include <ctime>
#include <iomanip>

#include "Draw.h"
#include "Render.h"

class SimpleGameEngine {
private:
    HINSTANCE  hInstance;
    Draw*      pDraw;
    Render*    pRender;
    Mesh       mCube;
    FMatrix3x3 mRotationX;
    FMatrix3x3 mRotationY;
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