#include "SimpleGameEngine.h"

SimpleGameEngine::SimpleGameEngine(HINSTANCE hInstance, int width, int height) : hInstance(NULL) {
    this->hInstance = hInstance;

    pDraw   = new Draw(hInstance, wndProc, this, 1920, 1080);
    pRender = new Render(width, height);

    mCube.triangles = {
        // Front
        {0.0f, 0.0f, 0.0f,   0.0f, 1.0f, 0.0f,   1.0f, 1.0f, 0.0f},
        {0.0f, 0.0f, 0.0f,   1.0f, 1.0f, 0.0f,   1.0f, 0.0f, 0.0f},

        // Back
        {0.0f, 0.0f, 1.0f,   0.0f, 1.0f, 1.0f,   1.0f, 1.0f, 1.0f},
        {0.0f, 0.0f, 1.0f,   1.0f, 1.0f, 1.0f,   1.0f, 0.0f, 1.0f},

        // Left
        {0.0f, 0.0f, 1.0f,   0.0f, 1.0f, 1.0f,   0.0f, 1.0f, 0.0f},
        {0.0f, 0.0f, 1.0f,   0.0f, 1.0f, 0.0f,   0.0f, 0.0f, 0.0f},

        // Right
        {1.0f, 0.0f, 0.0f,   1.0f, 1.0f, 0.0f,   1.0f, 1.0f, 1.0f},
        {1.0f, 0.0f, 0.0f,   1.0f, 1.0f, 1.0f,   1.0f, 0.0f, 1.0f},
        
        // Top
        {0.0f, 1.0f, 0.0f,   0.0f, 1.0f, 1.0f,   1.0f, 1.0f, 1.0f},
        {0.0f, 1.0f, 0.0f,   1.0f, 1.0f, 1.0f,   1.0f, 1.0f, 0.0f},

        // Bottom
        {1.0f, 0.0f, 1.0f,   0.0f, 0.0f, 1.0f,   0.0f, 0.0f, 0.0f},
        {1.0f, 0.0f, 1.0f,   0.0f, 0.0f, 0.0f,   1.0f, 0.0f, 0.0f},
    };

    
}

LRESULT CALLBACK SimpleGameEngine::wndProc(HWND hWnd, UINT uMessage, WPARAM wParam, LPARAM lParam) {
    LRESULT result                      = 0;
    BOOL    bHandled                    = false;
    SimpleGameEngine* pSimpleGameEngine = NULL;

    // If creation event, store SimpleGameEnine pointer as Window attribute
    if (uMessage == WM_CREATE) {
        pSimpleGameEngine = (SimpleGameEngine*)((LPCREATESTRUCT)lParam)->lpCreateParams;

        SetWindowLongPtr(hWnd, GWLP_USERDATA, (LONG_PTR)pSimpleGameEngine);

        result = 1;
    }
    // Otherwise process messages as normal
    else {
        pSimpleGameEngine = (SimpleGameEngine*)GetWindowLongPtr(hWnd, GWLP_USERDATA);

        if (pSimpleGameEngine) {
            switch (uMessage) {
                case WM_PAINT: {
                    Draw* pDraw = pSimpleGameEngine->getDraw();

                    if (pDraw) {
                        pDraw->beginPaint();
                        pSimpleGameEngine->draw();
                        pDraw->endPaint();
                    }

                    result = 0;
                    bHandled = true;
                }
                break;

                case WM_DESTROY: {
                    PostQuitMessage(0);
                    result = 0;
                    bHandled = true;
                }
                break;
            }
        }

        if (!bHandled)
            result = DefWindowProc(hWnd, uMessage, wParam, lParam);
    }

    return result;
}

int SimpleGameEngine::runWindowsMessageLoop() {
    MSG msg;

    while (GetMessage(&msg, NULL, 0, 0)) {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }

    return 0;
}

Draw* SimpleGameEngine::getDraw() {
    return this->pDraw;
}

void SimpleGameEngine::draw() {

    if (!epochTime)
        epochTime = std::time(nullptr);

    dTime += 0.03f;
    double dHalfTime = dTime * 0.5f;

    mRotationX.matrix[0][0] = 1;
    mRotationX.matrix[1][1] = std::cos(dTime);
    mRotationX.matrix[1][2] = -std::sin(dTime);
    mRotationX.matrix[2][1] = std::sin(dTime);
    mRotationX.matrix[2][2] = std::cos(dTime);

    mRotationY.matrix[0][0] = std::cos(dHalfTime);
    mRotationY.matrix[0][2] = std::sin(dHalfTime);
    mRotationY.matrix[1][1] = 1;
    mRotationY.matrix[2][0] = -std::sin(dHalfTime);
    mRotationY.matrix[2][2] = std::cos(dHalfTime);

    ID2D1SolidColorBrush* brush = pDraw->createBrush(255, 0, 0);

    for (auto triangle : mCube.triangles) {
        
        // Translate each point away from the origin
        FVector3D pt1Translated, pt2Translated, pt3Translated, translation = { 0, 0, 3 };

        pt1Translated = triangle.points[0] * mRotationX * mRotationY;
        pt2Translated = triangle.points[1] * mRotationX * mRotationY;
        pt3Translated = triangle.points[2] * mRotationX * mRotationY;

        pt1Translated = pt1Translated + translation;
        pt2Translated = pt2Translated + translation;
        pt3Translated = pt3Translated + translation;

        FVector2D pt1, pt2, pt3;

        pt1 = pRender->project(pt1Translated);
        pt2 = pRender->project(pt2Translated);
        pt3 = pRender->project(pt3Translated);

        pDraw->drawTriangle(pt1, pt2, pt3, brush);
    }

    framesRendered += 1;

    // Move this out of render loop
    if (std::time(nullptr) > epochTime) {
        char buffer[256];
        sprintf_s(buffer, 200, "FPS: %i", framesRendered);
        SetWindowTextA(pDraw->getHwnd(), buffer);

        epochTime      = std::time(nullptr);
        framesRendered = 0;
    }
}