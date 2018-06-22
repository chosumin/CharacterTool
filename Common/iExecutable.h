#pragma once

__interface iExecutable
{
	void Init();
	void Update();
	void PreRender();
	void Render();
	void PostRender();
	void ResizeScreen();
};