#pragma once

class cString
{
public:
	static wstring Wstring(string str);
	static string String(wstring wstr);

	static string Combine(vector<string> paths);
	static wstring Combine(vector<wstring> paths);

	static void Replace(string *str, string compare, string replace);
	static void Replace(wstring *str, wstring compare, wstring replace);
};