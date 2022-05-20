#pragma once

#include<vector>

#include "crttriangle.h"

class CRTMesh
{
public:
    CRTMesh(const std::vector<CRTVector> &vertices,
            const std::vector<int> &trianglesVertices,
            int materialIndex);

    int getMaterialIndex() const { return this->materialIndex; }

    std::vector<CRTTriangle> getTriangles() { return this->triangles; }

private:
    int materialIndex;
    std::vector<CRTTriangle> triangles;
};
