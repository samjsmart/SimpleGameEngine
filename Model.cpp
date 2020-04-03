#include "Model.h"

void Model::setMesh(Mesh mesh) {
    this->mesh = mesh;
}

void Model::setLocation(FVector3D location) {
    this->vLocation = location;
}

void Model::setRotation(float fPitch, float fYaw, float fRoll) {
    this->fPitch = fPitch;
    this->fYaw   = fYaw;
    this->fRoll = fRoll;
}

std::vector<Triangle> Model::getTriangles() {
    std::vector<Triangle> processedTriangles;
    bool                  bRotated  = false;
    FMatrix3x3            mRotation = { 
        1, 0, 0,
        0, 1, 0,
        0, 0, 1
    };

    // Calculate all rotation matrices
    if (fPitch) {
        FMatrix3x3 mYRotate = {
            cosf(fPitch),  0, sinf(fPitch),
            0,             1, 0,
            -sinf(fPitch), 0, cosf(fPitch)
        };

        bRotated  = true;
        mRotation = mRotation * mYRotate;
    }

    if (fYaw) {
        FMatrix3x3 mZRotate = {
            cosf(fYaw), -sinf(fYaw), 0,
            sinf(fYaw), cosf(fYaw),  0,
            0,          0,           1
        };

        bRotated = true;
        mRotation = mRotation * mZRotate;
    }

    if (fRoll) {
        FMatrix3x3 mXRotate = {
            1, 0,           0,
            0, cosf(fRoll), -sinf(fRoll),
            0, sinf(fRoll), cosf(fRoll)
        };

        bRotated = true;
        mRotation = mRotation * mXRotate;
    }

    for (auto triangle : mesh.triangles) {

        Triangle rotatedTranslatedTriangle = triangle;
        
        if (bRotated) {
            rotatedTranslatedTriangle.points[0] = rotatedTranslatedTriangle.points[0] * mRotation;
            rotatedTranslatedTriangle.points[1] = rotatedTranslatedTriangle.points[1] * mRotation;
            rotatedTranslatedTriangle.points[2] = rotatedTranslatedTriangle.points[2] * mRotation;
        }
        
        // Implement += operator for FVectors
        rotatedTranslatedTriangle.points[0] = rotatedTranslatedTriangle.points[0] + vLocation;
        rotatedTranslatedTriangle.points[1] = rotatedTranslatedTriangle.points[1] + vLocation;
        rotatedTranslatedTriangle.points[2] = rotatedTranslatedTriangle.points[2] + vLocation;

        processedTriangles.push_back(rotatedTranslatedTriangle);
    }

    return processedTriangles;
}