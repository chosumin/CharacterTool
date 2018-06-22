#include "stdafx.h"
#include "cDebug.h"

bool cDebug::_isDebug = true;
forward_list <function<void()>> cDebug::_logList;