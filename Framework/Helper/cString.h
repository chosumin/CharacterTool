#pragma once

class cString
{
public:
	static wstring Wstring(string str);
	static string String(wstring wstr);

	static string Combine(vector<string> paths);
	static wstring Combine(vector<wstring> paths);

	//지정한 문자열을 교체
	//@param : 전체 문자열
	//@param : 교체될 문자열
	//@param : 대체 문자열
	static void Replace(OUT string *str, string compare, string replace);

	//지정한 문자열을 교체
	//@param : 전체 문자열
	//@param : 교체될 문자열
	//@param : 대체 문자열
	static void Replace(OUT wstring *str, wstring compare, wstring replace);

	static void Split(OUT vector<string> *result, string origin, string tok);
	static void Split(OUT vector<wstring> *result, wstring origin, wstring tok);
};