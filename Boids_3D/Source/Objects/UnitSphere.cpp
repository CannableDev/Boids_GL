#include "UnitSphere.h"

#include <vector>
#define _USE_MATH_DEFINES
#include <math.h>
#include <iostream>
#include <fstream>
#include <string>

void Sphere::GeneratePoints(float radius, unsigned int detail)
{
    std::vector<float> vert;
    std::vector<unsigned int> indices;
    float x, y, z, xy;
    float s, t;

    float sectorStep = 2 * M_PI / detail;
    float stackStep = M_PI / detail;
    float sectorAngle, stackAngle;

    for (int i = 0; i <= detail; ++i)
    {
        stackAngle = M_PI / 2 - i * stackStep;        // starting from pi/2 to -pi/2
        xy = radius * cosf(stackAngle);             // r * cos(u)
        z = radius * sinf(stackAngle);              // r * sin(u)

        for (int j = 0; j <= detail; ++j)
        {
            sectorAngle = j * sectorStep;           // starting from 0 to 2pi

            // vertex position (x, y, z)
            x = xy * cosf(sectorAngle);             // r * cos(u) * cos(v)
            y = xy * sinf(sectorAngle);             // r * cos(u) * sin(v)

            vert.push_back(x);
            vert.push_back(y);
            vert.push_back(z);

        }
    }


    int k1, k2;
    for (int i = 0; i < detail; ++i)
    {
        k1 = i * (detail + 1);     // beginning of current stack
        k2 = k1 + detail + 1;      // beginning of next stack

        for (int j = 0; j < detail; ++j, ++k1, ++k2)
        {
            // 2 triangles per sector excluding first and last stacks
            // k1 => k2 => k1+1
            if (i != 0)
            {
                indices.push_back(k1);
                indices.push_back(k2);
                indices.push_back(k1 + 1);
            }

            // k1+1 => k2 => k2+1
            if (i != (detail - 1))
            {
                indices.push_back(k1 + 1);
                indices.push_back(k2);
                indices.push_back(k2 + 1);
            }
        }
    }


    std::ofstream file("sphere.txt", std::ofstream::out);

    file << "VERTS\n" << "{";

    for (int i = 0; i < vert.size(); i += 3) {
        file << "{ " << std::to_string(vert[i]) << ", "
            << std::to_string(vert[i + 1]) << ", "
            << std::to_string(vert[i + 2]) << " },\n";
    }

    file << "}\n\n\n";
    file << "INDICES\n" << "{";
    for (int i = 0; i < indices.size(); i += 3) {
        file << "{ " << std::to_string(indices[i]) << ", "
            << std::to_string(indices[i + 1]) << ", "
            << std::to_string(indices[i + 2]) << " },\n";
    }
    file << "}\n\n\n";
    file.close();
}
