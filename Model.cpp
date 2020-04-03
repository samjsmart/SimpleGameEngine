#include "Model.h"

void Model::setMesh(Mesh mesh) {
    this->mesh = mesh;
}

void Model::setLocation(FVector3D location) {
    this->vLocation = location;
}

void Model::setRotation(FRotator rRotation) {
    this->rRotation = rRotation;
}

std::vector<Triangle> Model::getTriangles() {
    std::vector<Triangle> processedTriangles;
    
    for (auto triangle : mesh.triangles) {

        Triangle rotatedTranslatedTriangle = triangle;
        
        rotatedTranslatedTriangle.points[0] *= rRotation;
        rotatedTranslatedTriangle.points[1] *= rRotation;
        rotatedTranslatedTriangle.points[2] *= rRotation;

        rotatedTranslatedTriangle.points[0] += vLocation;
        rotatedTranslatedTriangle.points[1] += vLocation;
        rotatedTranslatedTriangle.points[2] += vLocation;

        processedTriangles.push_back(rotatedTranslatedTriangle);
    }

    return processedTriangles;
}