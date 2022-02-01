#pragma once

#include <Core/MathTools.h>
#include <Core/BuildFlags.h>

#include <cstdlib>
#ifdef _DEBUG
#include <cassert>
#endif


#if ARRAY_BLOCK_THREADED_MALLOC
#include <Core/Threading/ThreadingUtils.h>
#define ARRAY_THREAD_GUARD() THREAD_GUARD()
#else
#define ARRAY_THREAD_GUARD() (void)0
#endif // !ALLOW_NON_THREADED_ARRAY_ALLOC


template <class Type> class TArray
{
private:
	Type* Data = nullptr;
	int32 AllocatedSize = 0;
	int32 UsedSize = 0;

public:
	TArray(int32 InitialSize = 0)
	{
		Prealocate(InitialSize);
	}

	~TArray()
	{
		if (Data)
		{
			ARRAY_THREAD_GUARD();
			DeleteData();
		}
	}

	TArray<Type>& operator= (const TArray<Type>& Copied)
	{
		if (Data)
		{
			DeleteData();
		}

		AllocatedSize = Copied.AllocatedSize;
		UsedSize = Copied.UsedSize;
		ARRAY_THREAD_GUARD();
		Data = (Type*)malloc(AllocatedSize * sizeof(Type));
		for (int32 Index = 0; Index < UsedSize; ++Index)
		{
			new (&Data[Index]) Type(Copied.Data[Index]);
		}

		return *this;
	}

	TArray(const TArray<Type>& Copied)
	{
		*this = Copied;
	}

	TArray<Type>& operator= (TArray<Type>&& Moved) noexcept
	{
		if (Data)
		{
			ARRAY_THREAD_GUARD();
			DeleteData();
		}

		AllocatedSize = Moved.AllocatedSize;
		UsedSize = Moved.UsedSize;
		Data = Moved.Data;

		Moved.AllocatedSize = 0;
		Moved.UsedSize = 0;
		Moved.Data = nullptr;

		return *this;
	}

	TArray(TArray<Type>&& Moved) noexcept
	{
		*this = Move(Moved);
	}

	void Prealocate(int32 Size)
	{
		if (Size == AllocatedSize)
		{
			return;
		}

		ARRAY_THREAD_GUARD();
		Type* NewData = Size > 0 ? (Type*)malloc(Size * sizeof(Type)) : nullptr;
		UsedSize = FMath::Min(Size, UsedSize);

		for (int32 Index = 0; Index < UsedSize; ++Index)
		{
			new (&NewData[Index]) Type(Move(Data[Index]));
		}
		if (Data)
		{
			DeleteData();
		}

		Data = NewData;
		AllocatedSize = Size;
	}

	Type& Push()
	{
		if (UsedSize == AllocatedSize)
		{
			Prealocate(AllocatedSize != 0 ? AllocatedSize * 2 : 16);
		}
		return *new (&Data[UsedSize++]) Type();
	}

	template <class... ArgsType>
	Type& Emplace(ArgsType... Args)
	{
		if (UsedSize == AllocatedSize)
		{
			Prealocate(AllocatedSize != 0 ? AllocatedSize * 2 : 16);
		}
		return *new (&Data[UsedSize++]) Type(Args...);
	}

	void Add(Type&& Added)
	{
		if (UsedSize == AllocatedSize)
		{
			Prealocate(AllocatedSize != 0 ? AllocatedSize * 2 : 16);
		}
		new (&Data[UsedSize++]) Type(Move(Added));
	}

	void Add(const Type& Added)
	{
		if (UsedSize == AllocatedSize)
		{
			Prealocate(AllocatedSize != 0 ? AllocatedSize * 2 : 16);
		}
		new (&Data[UsedSize++]) Type(Added);
	}

	Type&& Pop()
	{
		return Move(Data[UsedSize--]);
	}

	int32 Count() const
	{
		return UsedSize;
	}

	Type& operator[] (int32 Index)
	{
#ifdef _DEBUG
		assert(0 <= Index && Index < UsedSize);
#endif
		return Data[Index];
	}

	const Type& operator[] (int32 Index) const
	{
#ifdef _DEBUG
		assert(0 <= Index && Index < UsedSize);
#endif
		return Data[Index];
	}

	Type& Top()
	{
		return Data[UsedSize - 1];
	}

	void PopAll()
	{
		for (int32 Index = 0; Index < UsedSize; ++Index)
		{
			Data[Index].~Type();
		}
		UsedSize = 0;
	}

	void Clear()
	{
		if (Data)
		{
			DeleteData();
		}
		Data = nullptr;
		UsedSize = 0;
		AllocatedSize = 0;
	}

	int32 IndexOf(Type* Pointer)
	{
		const int32 Index = (int32)(Pointer - Data);
#if _DEBUG
		assert(0 <= Index && Index < UsedSize);
#endif
		return Index;
	}

	void LendData(Type* InData, int32 Size, bool bUsed = false)
	{
		Data = InData;
		AllocatedSize = Size;
		UsedSize = bUsed ? Size : 0;
	}

	void RemoveLend()
	{
		Data = nullptr;
		AllocatedSize = 0;
		UsedSize = 0;
	}

	void Insert(int32 Index, Type&& Inserted)
	{
		if (UsedSize == AllocatedSize)
		{
			Prealocate(AllocatedSize != 0 ? AllocatedSize * 2 : 16);
		}
		new (&Data[UsedSize++]) Type();
		for (int32 MoveIndex = UsedSize - 2; MoveIndex >= Index; --MoveIndex)
		{
			Data[MoveIndex + 1] = Move(Data[MoveIndex]);
		}
		Data[Index] = Move(Inserted);
	}

	void Insert(int32 Index, const Type& Added)
	{
		if (UsedSize == AllocatedSize)
		{
			Prealocate(AllocatedSize != 0 ? AllocatedSize * 2 : 16);
		}
		new (&Data[UsedSize++]) Type();
		for (int32 MoveIndex = UsedSize - 2; MoveIndex >= Index; --MoveIndex)
		{
			Data[MoveIndex + 1] = Move(Data[MoveIndex]);
		}
		Data[Index] = Added;
	}

private:
	void DeleteData()
	{
		if (Data)
		{
			for (int Index = 0; Index < UsedSize; ++Index)
			{
				Data[Index].~Type();
			}
			free(Data);
		}
	}

};

