#include "Render.h"

Render::Render(HINSTANCE hInstance, WNDPROC wndProc, LPVOID lpParam, int iWidth, int iHeight) {

    pDraw = new Draw(hInstance, wndProc, lpParam, iWidth, iHeight);

    this->iWidth  = iWidth;
    this->iHeight = iHeight;
    
    fAspectRatio = (float)iHeight / (float)iWidth;
    fNear        = 0.1f;
    fFov         = 90.0f;
    fFovRad      = 1.0f / std::tanf(fFov * 0.5f / 180.0f * 3.14159f);

    mProj.matrix[0][0] = fAspectRatio * fFovRad;
    mProj.matrix[1][1] = fFovRad;
    mProj.matrix[2][2] = fFar / (fFar - fNear);
    mProj.matrix[3][2] = (-fFar * fNear) / (fFar - fNear);
    mProj.matrix[2][3] = 1.0f;
    mProj.matrix[3][3] = 0.0f;
}

void Render::start() {
    pDraw->beginPaint();
}

void Render::finish() {
    pDraw->endPaint();
}

FVector2D Render::project(FVector3D point) {
    FVector2D ret;
    FVector4D pt4d = point;
    FVector4D projectedPoint;
    pt4d.W = 1;

    projectedPoint = pt4d * mProj;

    if (projectedPoint.W != 0.0f) {
        projectedPoint.X /= projectedPoint.W;
        projectedPoint.Y /= projectedPoint.W;
        projectedPoint.Z /= projectedPoint.W;
    }
 
    // Offset from -1 <-> 1 to 1 <-> 2;
    projectedPoint.X += 1.0f;
    projectedPoint.Y += 1.0f;
    projectedPoint.Z += 1.0f;

    // Add screen dimensions
    projectedPoint.X *= 0.5f * (float)iWidth;
    projectedPoint.Y *= 0.5f * (float)iHeight;

    ret.X = projectedPoint.X;
    ret.Y = projectedPoint.Y;

    return ret;
}

void Render::renderModel(FVector3D vCamera, Model model, bool bRenderMesh) {
    ID2D1SolidColorBrush* brush1 = pDraw->createBrush(255, 0, 0);
    ID2D1SolidColorBrush* brush2 = pDraw->createBrush(255, 255, 0);

    for (auto triangle : model.getTriangles()) {
        
        FVector3D vNormal     = triangle.normal();
        FVector3D vCameraLine = triangle.points[0] - vCamera;
        
        if (vNormal.dotProduct(vCameraLine) > 0.0)
            continue;

        FVector2D pt1, pt2, pt3;

        pt1 = project(triangle.points[0]);
        pt2 = project(triangle.points[1]);
        pt3 = project(triangle.points[2]);

        pDraw->drawFilledTriangle(pt1, pt2, pt3, brush1);
        //pDraw->drawTriangle(pt1, pt2, pt3, brush2);
    }
}