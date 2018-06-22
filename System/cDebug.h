#pragma once

class cDebug
{
public:
	static void Log(const type_info *type, function<void()> logFunction)
	{
		auto temp = type->name();
		if (_isDebug)
		{
			auto func = [temp, logFunction]()
			{
				cout << temp << endl;
				cout << "Debug : ";
				logFunction();
			};

			_logList.push_front(func);
		}
	}

	static bool GetDebugMode()
	{
		return _isDebug;
	}

	static void SetDebugMode(bool debug)
	{
		_isDebug = debug;
	}

	static void PrintLogs()
	{
		for (auto& log : _logList)
		{
			log();
			cout << endl;
		}
		_logList.clear();
	}
private:
	static bool _isDebug;
	static forward_list <function<void()>> _logList;
};