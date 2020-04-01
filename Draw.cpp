#include "Draw.h"

Draw::Draw(HINSTANCE hInstance, WNDPROC wndProc, LPVOID lpParam, int iWidth, int iHeight) :
    hWnd(NULL),
    wndProc(NULL),
    lpParam(NULL),
    pD2DFactory(NULL),
    pRenderTarget(NULL) {
    this->hInstance = hInstance;
    this->wndProc   = wndProc;
    this->lpParam   = lpParam;
    this->iWidth    = iWidth;
    this->iHeight   = iHeight;

    registerWindow();
    createD2D();
}

void Draw::registerWindow() {
    WNDCLASSEX wndClassEx = { sizeof(WNDCLASSEX) };
     
    wndClassEx.style         = CS_HREDRAW | CS_VREDRAW;
    wndClassEx.lpfnWndProc   = wndProc;
    wndClassEx.cbClsExtra    = 0;
    wndClassEx.cbWndExtra    = sizeof(LONG_PTR);
    wndClassEx.hInstance     = this->hInstance;
    wndClassEx.hbrBackground = NULL;
    wndClassEx.lpszClassName = NULL;
    wndClassEx.hCursor       = LoadCursor(NULL, IDI_APPLICATION);
    wndClassEx.lpszClassName = "My App";

    RegisterClassEx(&wndClassEx);
}

void Draw::createD2D() {
    D2D1CreateFactory(D2D1_FACTORY_TYPE_SINGLE_THREADED, &this->pD2DFactory);

    hWnd = CreateWindow(
        "My App",
        "My Application",
        WS_OVERLAPPEDWINDOW,
        CW_USEDEFAULT,
        CW_USEDEFAULT,
        iWidth,
        iHeight,
        NULL,
        NULL,
        hInstance,
        lpParam
    );

    ShowWindow(hWnd, SW_SHOWNORMAL);
    UpdateWindow(hWnd);

    RECT rect;
    GetClientRect(hWnd, &rect);

    pD2DFactory->CreateHwndRenderTarget(
        D2D1::RenderTargetProperties(),
        D2D1::HwndRenderTargetProperties(this->hWnd, D2D1::SizeU(rect.right - rect.left, rect.bottom - rect.top)),
        &pRenderTarget
    );
}

void Draw::beginPaint() {
    if (!pRenderTarget)
        return;

    ID2D1SolidColorBrush* brush = createBrush(255, 0, 0);

    pRenderTarget->BeginDraw();
    pRenderTarget->Clear(D2D1::ColorF(D2D1::ColorF::DarkBlue));
}

void Draw::endPaint() {
    pRenderTarget->EndDraw();
    clearBrushes();
}

ID2D1SolidColorBrush* Draw::createBrush(int r, int g, int b) {
    ID2D1SolidColorBrush* brush;

    pRenderTarget->CreateSolidColorBrush(D2D1::ColorF(r / 255.0f, g / 255.0f, b / 255.0f), &brush);
    vBrushes.push_back(brush);

    return brush;
}

void Draw::clearBrushes() {
    for (auto brush : vBrushes) {
        brush->Release();
    }

    vBrushes.clear();
}

void Draw::drawLine(FVector2D pt1, FVector2D pt2, ID2D1SolidColorBrush* pBrush, float fStrokeWidth) {
    pRenderTarget->DrawLine(D2D1::Point2F(pt1.X, pt1.Y), D2D1::Point2F(pt2.X, pt2.Y), pBrush, fStrokeWidth);
}

void Draw::drawTriangle(FVector2D pt1, FVector2D pt2, FVector2D pt3, ID2D1SolidColorBrush* pBrush, float fStrokeWidth) {
    drawLine(pt1, pt2, pBrush, fStrokeWidth);
    drawLine(pt2, pt3, pBrush, fStrokeWidth);
    drawLine(pt3, pt1, pBrush, fStrokeWidth);
}
