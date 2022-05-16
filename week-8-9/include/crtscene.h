#pragma once

#include <fstream>
#include <iostream>

#include "rapidjson\document.h"
#include "rapidjson\writer.h"
#include "rapidjson\stringbuffer.h"
#include "rapidjson\istreamwrapper.h"

#include "crtsettings.h"
#include "crtmesh.h"
#include "crtcamera.h"
#include "crtlight.h"

class CRTScene 
{
public:
    CRTScene() {}

    const std::vector<CRTMesh> &getGeometryObjects() const { return geometryObjects; }
    const CRTCamera &getCamera() const { return camera; }
    const CRTSettings &getSettings() const { return settings; }
    const std::vector<CRTLight> &getLights() const { return lights; }

    /// Read the content of the given file and store the data into scene members
    /// @param[in] sceneFileName = the scene file to read
    void parseSceneFile(const std::string &sceneFileName);

private:
    std::vector<CRTMesh> geometryObjects;
    std::vector<CRTLight> lights;
    CRTCamera camera;
    CRTSettings settings;

    const char *crtSceneSettings = "settings";
    const char *crtSceneBGColor = "background_color";
    const char *crtSceneImageSettings = "image_settings";
    const char *crtSceneImageWidth = "width";
    const char *crtSceneImageHeight = "height";
    const char *crtSceneGlobalAlbedo = "global_albedo";

    const char *crtSceneCamera = "camera";
    const char *crtSceneMatrix = "matrix";
    const char *crtScenePosition = "position";

    const char *crtObjects = "objects";
    const char *crtVertices = "vertices";
    const char *crtTriangles = "triangles";

    const char *crtLights = "lights";
    const char *crtLightIntensity = "intensity";
    const char *crtLightPosition = "position";
};