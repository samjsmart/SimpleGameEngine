#pragma once
#include <vector>

#include "Render.h"

class Model {
private:
    Mesh      mesh;
    FVector3D vLocation;
    FRotator  rRotation;

public:
    std::vector<Triangle> getTriangles();
    void setMesh(Mesh mesh);
    void setLocation(FVector3D position);
    void setRotation(FRotator rotation);
};