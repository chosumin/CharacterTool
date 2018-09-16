#pragma once

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
	static weak_ptr<class cCamera> MainCamera;
	static shared_ptr<class cPerspective> Perspective;
	static shared_ptr<class cViewport> Viewport;
	shared_ptr<class cViewProjectionBuffer> ViewProjection;
	shared_ptr<class cLightBuffer> GlobalLight;
	shared_ptr<struct sGuiSettings> GuiSettings;
};