#include "SimpleGameEngine.h"

SimpleGameEngine::SimpleGameEngine(HINSTANCE hInstance, int width, int height) : hInstance(NULL) {
    this->hInstance = hInstance;

    pRender = new Render(hInstance, wndProc, this, 1920, 1080);
    
    FVector3D pos = { 0.0f, 0.0f, 3.0f };
    Mesh      mCubeMesh;
    mCubeMesh.triangles = {
        // Front
        {FVector3D(0.0f, 0.0f, 0.0f),   FVector3D(0.0f, 1.0f, 0.0f),   FVector3D(1.0f, 1.0f, 0.0f)},
        {FVector3D(0.0f, 0.0f, 0.0f),   FVector3D(1.0f, 1.0f, 0.0f),   FVector3D(1.0f, 0.0f, 0.0f)},

        // Back
        {FVector3D(0.0f, 0.0f, 1.0f),   FVector3D(0.0f, 1.0f, 1.0f),   FVector3D(1.0f, 1.0f, 1.0f)},
        {FVector3D(0.0f, 0.0f, 1.0f),   FVector3D(1.0f, 1.0f, 1.0f),   FVector3D(1.0f, 0.0f, 1.0f)},

        // Left
        {FVector3D(0.0f, 0.0f, 1.0f),   FVector3D(0.0f, 1.0f, 1.0f),   FVector3D(0.0f, 1.0f, 0.0f)},
        {FVector3D(0.0f, 0.0f, 1.0f),   FVector3D(0.0f, 1.0f, 0.0f),   FVector3D(0.0f, 0.0f, 0.0f)},

        // Right
        {FVector3D(1.0f, 0.0f, 0.0f),   FVector3D(1.0f, 1.0f, 0.0f),   FVector3D(1.0f, 1.0f, 1.0f)},
        {FVector3D(1.0f, 0.0f, 0.0f),   FVector3D(1.0f, 1.0f, 1.0f),   FVector3D(1.0f, 0.0f, 1.0f)},
        
        // Top
        {FVector3D(0.0f, 1.0f, 0.0f),   FVector3D(0.0f, 1.0f, 1.0f),   FVector3D(1.0f, 1.0f, 1.0f)},
        {FVector3D(0.0f, 1.0f, 0.0f),   FVector3D(1.0f, 1.0f, 1.0f),   FVector3D(1.0f, 1.0f, 0.0f)},

        // Bottom
        {FVector3D(1.0f, 0.0f, 1.0f),   FVector3D(0.0f, 0.0f, 1.0f),   FVector3D(0.0f, 0.0f, 0.0f)},
        {FVector3D(1.0f, 0.0f, 1.0f),   FVector3D(0.0f, 0.0f, 0.0f),   FVector3D(1.0f, 0.0f, 0.0f)},
    };

    mCube.setMesh(mCubeMesh);
    mCube.setLocation(pos);
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
                    Render* pRender = pSimpleGameEngine->getRender();

                    if (pRender) {
                        pRender->start();
                        pSimpleGameEngine->update();
                        pRender->finish();
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

Render* SimpleGameEngine::getRender() {
    return pRender;
}

int SimpleGameEngine::runWindowsMessageLoop() {
    MSG msg;

    while (GetMessage(&msg, NULL, 0, 0)) {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }

    return 0;
}

void SimpleGameEngine::update() {

    if (!epochTime)
        epochTime = (double)std::time(nullptr);

    dTime += 0.03f;
    double dHalfTime = dTime * 0.5f;
    FRotator rRotation((float)dTime, (float)dHalfTime, 0);

    mCube.setRotation(rRotation);
    pRender->renderModel(mCube);

    framesRendered += 1;

    // Move this out of render loop
    if (std::time(nullptr) > epochTime) {
        char buffer[256];
        sprintf_s(buffer, 200, "FPS: %i", framesRendered);
        //SetWindowTextA(pDraw->getHwnd(), buffer);

        epochTime      = (double)std::time(nullptr);
        framesRendered = 0;
    }
}