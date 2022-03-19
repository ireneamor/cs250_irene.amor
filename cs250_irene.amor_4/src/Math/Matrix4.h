#ifndef MATRIX4_H
#define MATRIX4_H

#include <cstdio>              // printf
#include "Vector4.h"
#include "Point4.h"

class Matrix4
{
    public:
        
        union
        {
            float m[4][4];
            float v[16];
        };
        /*
            This union lets us access the data in multiple ways
            All of these are modifying the same location in memory
            Matrix4 mtx;
            mtx.m[2][2] = 1.0f;
            mtx.v[10] = 2.0f;
        */

        // Default constructor should initialize to zeroes
        Matrix4(void);

        // Copy constructor, copies every entry from the other matrix.
        Matrix4(const Matrix4& rhs);

        // Non-default constructor, self-explanatory
        Matrix4(float mm00, float mm01, float mm02, float mm03,
        float mm10, float mm11, float mm12, float mm13,
        float mm20, float mm21, float mm22, float mm23,
        float mm30, float mm31, float mm32, float mm33);

        // Assignment operator, does not need to handle self-assignment
        Matrix4& operator=(const Matrix4& rhs);

        // Multiplying a Matrix4 with a Vector4 or a Point4
        Vector4 operator*(const Vector4& rhs) const;
        Point4 operator*(const Point4& rhs) const;

        // Basic Matrix arithmetic operations
        Matrix4 operator+(const Matrix4& rhs) const;
        Matrix4 operator-(const Matrix4& rhs) const;
        Matrix4 operator*(const Matrix4& rhs) const;

        // Similar to the three above except they modify the original
        Matrix4& operator+=(const Matrix4& rhs);
        Matrix4& operator-=(const Matrix4& rhs);
        Matrix4& operator*=(const Matrix4& rhs);

        // Scale/Divide the entire matrix by a float
        Matrix4 operator*(const float rhs) const;
        Matrix4 operator/(const float rhs) const;
        // Same as previous
        Matrix4& operator*=(const float rhs);
        Matrix4& operator/=(const float rhs);

        // Comparison operators which should use an epsilon defined in
        // MathUtilities.h to see if the value is within a certain range
        // in which case we say they are equivalent.
        bool operator==(const Matrix4& rhs) const;
        bool operator!=(const Matrix4& rhs) const;

        // Zeroes out the entire matrix
        void Zero(void);

        // Builds the identity matrix
        void Identity(void);

        // Already implemented, simple print function
        void Print(void) const
        {
            std::printf("--------------------------\n");
            std::printf("%5.3f %5.3f %5.3f %5.3f\n", m[0][0], m[0][1], m[0][2], m[0][3]);
            std::printf("%5.3f %5.3f %5.3f %5.3f\n", m[1][0], m[1][1], m[1][2], m[1][3]);
            std::printf("%5.3f %5.3f %5.3f %5.3f\n", m[2][0], m[2][1], m[2][2], m[2][3]);
            std::printf("%5.3f %5.3f %5.3f %5.3f\n", m[3][0], m[3][1], m[3][2], m[3][3]);
            std::printf("--------------------------\n");
        }
};

#endif