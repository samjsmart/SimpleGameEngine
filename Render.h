#pragma once
#include <vector>

#include "Math.h"
#include "Model.h"
#include "Draw.h"

#include <iostream>

class Render {
private:
    Draw*      pDraw;
    int        iWidth, iHeight;
    float      fAspectRatio, fNear, fFar, fFov, fFovRad;
    FMatrix4x4 mProj;

public:
    Render(HINSTANCE hInstance, WNDPROC wndProc, LPVOID lpParam, int iWidth, int iHeight);
    FVector2D project(FVector3D point);
    void renderModel(Model model, bool bRenderMesh = false);
    void start();
    void finish();
};