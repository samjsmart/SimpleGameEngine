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

    pRenderTarget->BeginDraw();
    pRenderTarget->Clear(D2D1::ColorF(D2D1::ColorF::Black));
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

HWND Draw::getHwnd() {
    return hWnd;
}

void Draw::drawLine(float x1, float y1, float x2, float y2, ID2D1SolidColorBrush* pBrush) {
    pRenderTarget->DrawLine(D2D1::Point2F(x1, y1), D2D1::Point2F(x2, y2), pBrush);
}

void Draw::drawLine(FVector2D pt1, FVector2D pt2, ID2D1SolidColorBrush* pBrush) {
    pRenderTarget->DrawLine(D2D1::Point2F(pt1.X, pt1.Y), D2D1::Point2F(pt2.X, pt2.Y), pBrush);
}

void Draw::drawTriangle(FVector2D pt1, FVector2D pt2, FVector2D pt3, ID2D1SolidColorBrush* pBrush) {
    drawLine(pt1, pt2, pBrush);
    drawLine(pt2, pt3, pBrush);
    drawLine(pt3, pt1, pBrush);
}

void Draw::fillFlatBottomTriangle(FVector2D pt1, FVector2D pt2, FVector2D pt3, ID2D1SolidColorBrush* pBrush) {
    float fSloap1 = (pt2.X - pt1.X) / (pt2.Y - pt1.Y);
    float fSloap2 = (pt3.X - pt1.X) / (pt3.Y - pt1.Y);

    float fCurrentX1 = pt1.X;
    float fCurrentX2 = pt1.X;

    for (int iScanY = pt1.Y; iScanY <= pt2.Y; iScanY++) {
        drawLine(fCurrentX1, iScanY, fCurrentX2, iScanY, pBrush);
        fCurrentX1 += fSloap1;
        fCurrentX2 += fSloap2;
    }
}

void Draw::fillFlatTopTriangle(FVector2D pt1, FVector2D pt2, FVector2D pt3, ID2D1SolidColorBrush* pBrush) {
    float fSloap1 = (pt3.X - pt1.X) / (pt3.Y - pt1.Y);
    float fSloap2 = (pt3.X - pt2.X) / (pt3.Y - pt2.Y);

    float fCurrentX1 = pt3.X;
    float fCurrentX2 = pt3.X;

    for (int iScanY = pt3.Y; iScanY > pt1.Y; iScanY--) {
        drawLine(fCurrentX1, iScanY, fCurrentX2, iScanY, pBrush);
        fCurrentX1 -= fSloap1;
        fCurrentX2 -= fSloap2;
    }
}

void Draw::drawFilledTriangle(FVector2D pt1, FVector2D pt2, FVector2D pt3, ID2D1SolidColorBrush* pBrush) {
    // http://www.sunshine2k.de/coding/java/TriangleRasterization/TriangleRasterization.html

    FVector2D* pPt1 = &pt1;
    FVector2D* pPt2 = &pt2;
    FVector2D* pPt3 = &pt3;

    if (pPt1->Y > pPt2->Y)
        std::swap(pPt1, pPt2);
    if (pPt2->Y > pPt3->Y)
        std::swap(pPt2, pPt3);
    if (pPt1->Y > pPt2->Y)
        std::swap(pPt1, pPt2);

    if (pPt2->Y == pPt3->Y) {
        fillFlatBottomTriangle(*pPt1, *pPt2, *pPt3, pBrush);
    }
    else if (pPt1->Y == pPt2->Y) {
        fillFlatTopTriangle(*pPt1, *pPt2, *pPt3, pBrush);
    }
    else {
        FVector2D pt4 = {
            pt1.X + ((pPt2->Y - pPt1->Y) / (pPt3->Y - pPt1->Y)) * (pPt3->X - pPt3->X),
            pPt2->Y
        };

        fillFlatBottomTriangle(*pPt1, *pPt2, pt4, pBrush);
        fillFlatTopTriangle(*pPt2, pt4, *pPt3, pBrush);
    }
}