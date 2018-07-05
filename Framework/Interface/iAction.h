#pragma once

class iAction
{
public:
	virtual ~iAction() {}

	virtual void Idle() = 0;
	virtual void Move() = 0;
};