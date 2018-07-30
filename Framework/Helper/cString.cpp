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
	���ڿ� ����
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
compare�� replace�� ����
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

	int cutAt = 0; //�ڸ� ��ġs
	while ((cutAt = (int)origin.find_first_of(tok)) != origin.npos)
	{
		if (cutAt > 0) //�ڸ��� ��ġ�� 0����ũ��
			result->push_back(origin.substr(0, cutAt));

		origin = origin.substr(cutAt + 1);
	}

	if (origin.length() > 0) //�ڸ��� ���� ���� �ִٸ�
		result->push_back(origin.substr(0, cutAt));
}

void cString::Split(OUT vector<wstring> * result, wstring origin, wstring tok)
{
	result->clear();

	int cutAt = 0; //�ڸ� ��ġs
	while ((cutAt = (int)origin.find_first_of(tok)) != origin.npos)
	{
		if (cutAt > 0) //�ڸ��� ��ġ�� 0����ũ��
			result->push_back(origin.substr(0, cutAt));

		origin = origin.substr(cutAt + 1);
	}

	if (origin.length() > 0) //�ڸ��� ���� ���� �ִٸ�
		result->push_back(origin.substr(0, cutAt));
}
