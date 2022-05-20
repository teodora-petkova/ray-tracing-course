#pragma once

#include "crtcolor.h"
#include "crtvector.h"

enum CRTMaterialType
{
    diffuse = 0,
    reflective = 1
};

class CRTMaterial
{
public:
    CRTMaterial()
        : type(CRTMaterialType::diffuse), albedo(CRTColor(0, 1, 1)), smooth(false)
    {
    }

    CRTMaterial(const char* type, CRTColor albedo, bool isSmooth)
        : albedo(albedo), smooth(isSmooth)
    {
        if(strcmp(type, "diffuse") == 0) { 
            this->type = CRTMaterialType::diffuse; 
        }
        else if(strcmp(type, "reflective") == 0) {
            this->type = CRTMaterialType::reflective; 
        }
        else {
            this->type = CRTMaterialType::diffuse;
        }
    }

    CRTColor getAlbedo() const { return albedo; }
    CRTMaterialType getType() const { return type; }
    bool isSmooth() const { return smooth; }

private:
    CRTMaterialType type;
    CRTColor albedo;
    bool smooth;
};