#pragma once
#include "./Viewer/cCamera.h"
#include "./Viewer/cPerspective.h"
#include "./Viewer/cViewport.h"

struct sGlobalVariable
{
	shared_ptr<class cViewProjectionBuffer> ViewProjection;
	shared_ptr<class cPerspective> Perspective;
	shared_ptr<class cViewport> Viewport;
	shared_ptr<class cCamera> MainCamera;
	shared_ptr<class cLightBuffer> GlobalLight;
};