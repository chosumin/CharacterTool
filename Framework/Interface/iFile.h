#pragma once
#include "stdafx.h"

class iFile
{
public:
	virtual ~iFile() {}
	virtual void LoadJson(Json::Value& root) = 0;
	virtual void SaveJson(Json::Value& root) = 0;
};
