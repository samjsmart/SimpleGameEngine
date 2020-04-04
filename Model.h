#pragma once
#include <vector>
#include "Math.h"

struct Triangle {
    FVector3D points[3];
};

struct Mesh {
    std::vector<Triangle> triangles;
};

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