#include "stdafx.h"
#include "cString.h"

wstring cString::Wstring(string str)
{
	wstring temp = L"";
	temp.assign(str.begin(), str.end());
	return temp;
}

string cString::String(wstring wstr)
{
	string temp = "";
	temp.assign(wstr.begin(), wstr.end());
	return temp;
}

/*****************
	문자열 결합
******************/
string cString::Combine(vector<string> paths)
{
	string temp = "";
	for (string path : paths)
		temp += path;

	return temp;
}

wstring cString::Combine(vector<wstring> paths)
{
	wstring temp = L"";
	for (wstring path : paths)
		temp += path;

	return temp;
}

/***********************
compare를 replace로 변경
************************/
void cString::Replace(OUT string * str, string compare, string replace)
{
	string temp = *str;

	size_t start_pos = 0;
	while ((start_pos = temp.find(compare, start_pos)) != wstring::npos)
	{
		temp.replace(start_pos, compare.length(), replace);
		start_pos += replace.length();
	}

	*str = temp;
}

void cString::Replace(OUT wstring * str, wstring compare, wstring replace)
{
	wstring temp = *str;

	size_t start_pos = 0;
	while ((start_pos = temp.find(compare, start_pos)) != wstring::npos)
	{
		temp.replace(start_pos, compare.length(), replace);
		start_pos += replace.length();
	}

	*str = temp;
}

void cString::Split(OUT vector<string> * result, string origin, string tok)
{
	result->clear();

	int cutAt = 0; //자를 위치s
	while ((cutAt = (int)origin.find_first_of(tok)) != origin.npos)
	{
		if (cutAt > 0) //자르는 위치가 0보다크면
			result->push_back(origin.substr(0, cutAt));

		origin = origin.substr(cutAt + 1);
	}

	if (origin.length() > 0) //자르고도 남은 것이 있다면
		result->push_back(origin.substr(0, cutAt));
}

void cString::Split(OUT vector<wstring> * result, wstring origin, wstring tok)
{
	result->clear();

	int cutAt = 0; //자를 위치s
	while ((cutAt = (int)origin.find_first_of(tok)) != origin.npos)
	{
		if (cutAt > 0) //자르는 위치가 0보다크면
			result->push_back(origin.substr(0, cutAt));

		origin = origin.substr(cutAt + 1);
	}

	if (origin.length() > 0) //자르고도 남은 것이 있다면
		result->push_back(origin.substr(0, cutAt));
}
