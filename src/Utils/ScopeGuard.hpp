#pragma once

template<typename Callback>
class ExecuteOnExitingScope
{
public:
	ExecuteOnExitingScope(Callback func) : _func(func) {}
	~ExecuteOnExitingScope()
	{
		_func();
	}
	
private:
	Callback _func;
};

template<typename Callback>
ExecuteOnExitingScope<Callback> Create(Callback func)
{
	return ExecuteOnExitingScope<Callback>(func);
}

#define _CONCAT(A, B) A##B
#define _OnExitingScope(FUNC, LINE) auto _CONCAT(_test_, LINE) = Create FUNC
#define OnExitingScope(FUNC) _OnExitingScope(FUNC, __LINE__)