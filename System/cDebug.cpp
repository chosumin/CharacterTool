#include "stdafx.h"
#include "cDebug.h"

bool cDebug::_isDebug = true;
forward_list <function<void()>> cDebug::_logList;
ImVector<char*> cDebug::Items;
ImVector<char*> cDebug::Items2;

void cDebug::PrintLogs()
{
	static int tab = 0;
	for (auto& log : _logList)
	{
		log();
		cout << endl;
	}
	_logList.clear();

	ImGui::Begin("Console", nullptr, ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoResize);
	{
		ImGui::RadioButton("Stack", &tab, 0);
		ImGui::SameLine();
		ImGui::RadioButton("Renew", &tab, 1);

		ImGui::Separator();

		if (tab == 0)
		{
			//todo : 클리어 버튼, 로그 식별자 만들기
			int size = Items.size();
			for (int i = size - 1; i >= 0; i--)
				ImGui::TextUnformatted(Items[i]);
		}
		else if (tab == 1)
		{
			int size = Items2.size();
			for (int i = size - 1; i >= 0; i--)
				ImGui::TextUnformatted(Items2[i]);
			Items2.clear();
		}
	}
	ImGui::End();
}

char * cDebug::Strdup(const char * str)
{
	size_t len = strlen(str) + 1;
	void* buff = malloc(len);
	return (char*)memcpy(buff, (const void*)str, len);
}

void cDebug::Log(const char* fmt, ...) IM_FMTARGS(2)
{
	char buf[1024];
	va_list args;
	va_start(args, fmt);
	vsnprintf(buf, IM_ARRAYSIZE(buf), fmt, args);
	buf[IM_ARRAYSIZE(buf) - 1] = 0;
	va_end(args);
	Items.push_back(Strdup(buf));
}

void cDebug::RenewalLog(const char* fmt, ...) IM_FMTARGS(2)
{
	char buf[1024];
	va_list args;
	va_start(args, fmt);
	vsnprintf(buf, IM_ARRAYSIZE(buf), fmt, args);
	buf[IM_ARRAYSIZE(buf) - 1] = 0;
	va_end(args);
	Items2.push_back(Strdup(buf));
}