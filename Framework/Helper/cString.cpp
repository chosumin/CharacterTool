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
void cString::Replace(string * str, string compare, string replace)
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

void cString::Replace(wstring * str, wstring compare, wstring replace)
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
