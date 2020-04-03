#pragma once
#include "Render.h"

class Model {
private:
    Mesh mesh;
    float fPitch, fYaw, fRoll;
    FVector3D vLocation;

public:
    std::vector<Triangle> getTriangles();
    void setMesh(Mesh mesh);
    void setLocation(FVector3D position);
    void setRotation(float fPitch, float fYaw, float fRoll);
};