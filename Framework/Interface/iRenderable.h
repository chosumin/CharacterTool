#pragma once

class iRenderable
{
public:
	virtual ~iRenderable() {}
	virtual void Render() = 0;
};