#pragma once
#include <vector>
#include "Math.h"

#include <iostream>

struct Triangle {
    FVector3D points[3];
};

struct Mesh {
    std::vector<Triangle> triangles;
};

class Render {
private:
    int        iWidth, iHeight;
    float      fAspectRatio, fNear, fFar, fFov, fFovRad;
    FMatrix4x4 mProj;

public:
    Render(int width, int height);
    FVector2D project(FVector3D point);
};