#pragma once
#include <vector>
#include "Math.h"

struct Triangle {
    FVector3D points[3];

    FVector3D normal() {
        FVector3D vLineA = points[1] - points[0];
        FVector3D vLineB = points[2] - points[0];

        FVector3D vNormal = {
            vLineA.Y * vLineB.Z - vLineA.Z * vLineB.Y,
            vLineA.Z * vLineB.X - vLineA.X * vLineB.Z,
            vLineA.X * vLineB.Y - vLineA.Y * vLineB.X
        };

        float fLength = sqrtf(vNormal.X * vNormal.X + vNormal.Y * vNormal.Y + vNormal.Z * vNormal.Z);

        return vNormal /= fLength;
    }
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