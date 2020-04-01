#include "Render.h"

Render::Render(int width, int height) {
    this->iWidth  = width;
    this->iHeight = height;
    
    fAspectRatio = (float)height / (float)width;
    fFar         = 1000.0f;
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