#pragma once
#include <Windows.h>
#include <d2d1.h>
#include <d2d1helper.h>
#include <dwrite.h>
#include <vector>

#include "Math.h"

//Remove when logging class is created
#include <iostream>


class Draw {
private:
    WNDPROC                            wndProc;
    LPVOID                             lpParam;
    HWND                               hWnd;
    HINSTANCE                          hInstance;
    ID2D1Factory*                      pD2DFactory;
    ID2D1HwndRenderTarget*             pRenderTarget;
    int                                iWidth, iHeight;
    std::vector<ID2D1SolidColorBrush*> vBrushes;

    void registerWindow();
    void createD2D();
    void clearBrushes();
    
public:
    Draw(HINSTANCE hInstance, WNDPROC wndProc, LPVOID lpParam, int iWidth, int iHeight);
    ID2D1SolidColorBrush* createBrush(int r, int g, int b);
    void drawLine(FVector2D pt1, FVector2D pt2, ID2D1SolidColorBrush* pBrush, float fStrokeWidth = 1.0f);
    void drawTriangle(FVector2D pt1, FVector2D pt2, FVector2D pt3, ID2D1SolidColorBrush* pBrush, float fStrokeWidth = 1.0f);
    void beginPaint();
    void endPaint();
};