#include "stdafx.h"
#include "cMenuBar.h"

cMenuBar::cMenuBar(weak_ptr<sGlobalVariable> global)
	:_global(global)
{
}

cMenuBar::~cMenuBar()
{
}

void cMenuBar::Init()
{
}

void cMenuBar::Update()
{
	if (cKeyboard::Get()->Press(VK_CONTROL))
	{
		if (cKeyboard::Get()->Down(VK_F1))
			ChangeShowEnvironmentWindow();

		if (cKeyboard::Get()->Down(VK_F2))
			ChangeShowLandscapeWindow();

		if (cKeyboard::Get()->Down(VK_F10))
			ChangeShowGlobalLightWindow();

		if (cKeyboard::Get()->Down(VK_F11))
			ChangeShowSystemInfoWindow();

		if (cKeyboard::Get()->Down(VK_F12))
			ChangeShowDemoWindow();
	}
}

void cMenuBar::PostRender()
{
	ShowMenuBar();
	ShowGlobalLight();
	ShowDemo();
}

void cMenuBar::ShowMenuBar()
{
	if (ImGui::BeginMainMenuBar())
	{
		if (ImGui::BeginMenu("View"))
		{
			if (ImGui::MenuItem("Environment", "Ctrl+F1"))
				ChangeShowEnvironmentWindow();

			if (ImGui::MenuItem("Landscape", "Ctrl+F2"))
				ChangeShowLandscapeWindow();

			if (ImGui::MenuItem("Global Light", "Ctrl+F10"))
				ChangeShowGlobalLightWindow();

			if (ImGui::MenuItem("SystemInfo", "Ctrl+F11"))
				ChangeShowSystemInfoWindow();

			if (ImGui::MenuItem("Demo", "Ctrl+F12"))
				ChangeShowDemoWindow();
			ImGui::EndMenu();
		}
		ImGui::EndMainMenuBar();
	}
}

void cMenuBar::ShowGlobalLight()
{
	auto globalPtr = _global.lock();
	if (globalPtr->GuiSettings->bShowGlobalLightWindow == true)
	{
		ImGui::Begin("Global Light", &globalPtr->GuiSettings->bShowGlobalLightWindow);

		ImGui::SliderFloat3("Sun Direction"
							, globalPtr->GlobalLight->Data.Direction
							, -1, 1);
		ImGui::ColorEdit4("Color", globalPtr->GlobalLight->Data.Color);
		ImGui::SliderFloat("Overcast",
						   &globalPtr->GlobalLight->Data.Overcast
						   , 0.0001f, 1.0f);
		ImGui::End();
	}
}

void cMenuBar::ShowDemo()
{
	auto globalPtr = _global.lock();
	if (globalPtr->GuiSettings->bShowDemoWindow == true)
	{
		ImGui::ShowDemoWindow(&globalPtr->GuiSettings->bShowDemoWindow);
	}
}

void cMenuBar::ChangeShowGlobalLightWindow()
{
	auto globalPtr = _global.lock();
	globalPtr->GuiSettings->bShowGlobalLightWindow =
		!globalPtr->GuiSettings->bShowGlobalLightWindow;
}

void cMenuBar::ChangeShowEnvironmentWindow()
{
	auto globalPtr = _global.lock();
	globalPtr->GuiSettings->bShowEnvironmentWindow =
		!globalPtr->GuiSettings->bShowEnvironmentWindow;
}

void cMenuBar::ChangeShowLandscapeWindow()
{
	auto globalPtr = _global.lock();
	globalPtr->GuiSettings->bShowLandscapeWindow =
		!globalPtr->GuiSettings->bShowLandscapeWindow;
}

void cMenuBar::ChangeShowSystemInfoWindow()
{
	auto globalPtr = _global.lock();
	globalPtr->GuiSettings->bShowSystemInfoWindow =
		!globalPtr->GuiSettings->bShowSystemInfoWindow;
}

void cMenuBar::ChangeShowDemoWindow()
{
	auto globalPtr = _global.lock();
	globalPtr->GuiSettings->bShowDemoWindow =
		!globalPtr->GuiSettings->bShowDemoWindow;
}