#pragma once
#include "./System/D3D.h"
#include "./Graphic/cStates.h"
#include "./Viewer/cCamera.h"
#include "./Viewer/cPerspective.h"
#include "./Viewer/cViewport.h"

struct sGuiSettings
{
	bool bShowGlobalLightWindow = false;
	bool bShowEnvironmentWindow = false;
	bool bShowLandscapeWindow = false;
	bool bShowSystemInfoWindow = false;
	bool bShowDemoWindow = false;
	bool bShowHierarchy = true;
	bool bShowInspector = true;
};

struct sGlobalVariable
{
	shared_ptr<class cViewProjectionBuffer> ViewProjection;
	shared_ptr<class cPerspective> Perspective;
	shared_ptr<class cViewport> Viewport;
	weak_ptr<class cCamera> MainCamera;
	shared_ptr<class cLightBuffer> GlobalLight;
	shared_ptr<struct sGuiSettings> GuiSettings;
};