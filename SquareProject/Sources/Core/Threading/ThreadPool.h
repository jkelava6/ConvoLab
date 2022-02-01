#pragma once

#include <Core/DynamicArray.h>
#include <Core/Threading/ThreadInclude.h>

class FThreadPool
{
public:
	FThreadPool();
	~FThreadPool();
	DECLARE_MOVE(FThreadPool);
	DECLARE_NOCOPY(FThreadPool);

public:
	void ResizeBlocking(int Size);
	bool AllThreadsFree();
	void WaitUntilDone();
	int ExecuteTaskThreaded(FunctionPointer(void, Task, void*), void* Arg);

public:
	TArray<bool> Busy;

private:
	TArray<FCppThread> Threads;
};
