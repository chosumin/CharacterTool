#pragma once

class cString
{
public:
	static wstring Wstring(string str);
	static string String(wstring wstr);

	static string Combine(vector<string> paths);
	static wstring Combine(vector<wstring> paths);

	static void Replace(OUT string *str, string compare, string replace);
	static void Replace(OUT wstring *str, wstring compare, wstring replace);

	static void Split(OUT vector<string> *result, string origin, string tok);
	static void Split(OUT vector<wstring> *result, wstring origin, wstring tok);
};