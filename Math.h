#pragma once

struct FMatrix4x4 {
    float matrix[4][4] = { 0 };
};

struct FMatrix3x3 {
    float matrix[3][3] = { 0 };
};

struct FVector2D {
    float X, Y;

    FVector2D() {
        X = 0;
        Y = 0;
    }

    FVector2D(float x, float y) {
        X = x;
        Y = y;
    }
};

struct FVector3D {
    float X, Y, Z;

    FVector3D& operator+(const FVector3D& rhs) {
        X += rhs.X;
        Y += rhs.Y;
        Z += rhs.Z;

        return *this;
    }

    FVector3D& operator*(const FMatrix3x3& rhs) {
        float newX = X * rhs.matrix[0][0] + Y * rhs.matrix[1][0] + Z * rhs.matrix[2][0];
        float newY = X * rhs.matrix[0][1] + Y * rhs.matrix[1][1] + Z * rhs.matrix[2][1];
        float newZ = X * rhs.matrix[0][2] + Y * rhs.matrix[1][2] + Z * rhs.matrix[2][2];

        X = newX;
        Y = newY;
        Z = newZ;

        return *this;
    }
};

struct FVector4D {
    float X, Y, Z, W;

    FVector4D() {
        X = 0;
        Y = 0;
        Z = 0;
        W = 0;
    }

    FVector4D(const FVector3D& rhs) {
        X = rhs.X;
        Y = rhs.Y;
        Z = rhs.Z;
        W = 0;
    }

    FVector4D& operator=(const FVector3D& rhs) {
        X = rhs.X;
        Y = rhs.Y;
        Z = rhs.Z;
        W = 0;

        return *this;
    }

    FVector4D& operator*(const FMatrix4x4& rhs) {
        float newX = X * rhs.matrix[0][0] + Y * rhs.matrix[1][0] + Z * rhs.matrix[2][0] + W * rhs.matrix[3][0];
        float newY = X * rhs.matrix[0][1] + Y * rhs.matrix[1][1] + Z * rhs.matrix[2][1] + W * rhs.matrix[3][1];
        float newZ = X * rhs.matrix[0][2] + Y * rhs.matrix[1][2] + Z * rhs.matrix[2][2] + W * rhs.matrix[3][2];
        float newW = X * rhs.matrix[0][3] + Y * rhs.matrix[1][3] + Z * rhs.matrix[2][3] + W * rhs.matrix[3][3];

        X = newX;
        Y = newY;
        Z = newZ;
        W = newW;

        return *this;
    }
};
