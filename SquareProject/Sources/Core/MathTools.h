#pragma once

#include <cmath>

extern uint32 RandomSeed;

class FMath
{
public:
	template <class Type>
	static inline Type Max(Type One, Type Other)
	{
		return One > Other ? One : Other;
	}

	template <class Type>
	static inline Type Min(Type One, Type Other)
	{
		return One < Other ? One : Other;
	}

	static inline int AbsI(int EvaluateAt)
	{
		return EvaluateAt >= 0 ? EvaluateAt : -EvaluateAt;
	}

	static inline float AbsF(float EvaluateAt)
	{
		return EvaluateAt >= 0.0f ? EvaluateAt : -EvaluateAt;
	}

	static inline float ClampF(float EvaluateAt, float MinVal, float MaxVal)
	{
		return Max(Min(EvaluateAt, MaxVal), MinVal);
	}

	static inline int32 ClampI(int32 EvaluateAt, int32 MinVal, int32 MaxVal)
	{
		return Max(Min(EvaluateAt, MaxVal), MinVal);
	}

	static inline float LerpF(float From, float To, float Distance)
	{
		return From + Distance * (To - From);
	}

	static inline float SquareF(float Squared)
	{
		return Squared * Squared;
	}

	static inline int32 SquareI(int32 Squared)
	{
		return Squared * Squared;
	}

	static inline float SignF(float EvaluateAt)
	{
		if (EvaluateAt < 0.0f)
		{
			return -1.0f;
		}
		if (EvaluateAt > 0.0f)
		{
			return 1.0f;
		}
		return 0.0;
	}

	static inline float PowerNat(float Exp)
	{
		return powf(2.71828182846f, Exp);
	}

	static inline float Power(float Base, float Exp)
	{
		return powf(Base, Exp);
	}
	
	static inline float LogNat(float Value)
	{
		return logf(Value);
	}

private:
	// Wang has random number generation, taken from:
	// https://www.reedbeta.com/blog/quick-and-easy-gpu-random-numbers-in-d3d11/
	static inline uint32 GenerateWangHash(uint32 Seed)
	{
		Seed = (Seed ^ 61) ^ (Seed >> 16);
		Seed *= 9;
		Seed = Seed ^ (Seed >> 4);
		Seed *= 0x27d4eb2d;
		Seed = Seed ^ (Seed >> 15);
		return Seed;
	}

public:
	static inline void SetRandomSeed(uint32 NewSeed)
	{
		RandomSeed = NewSeed;
	}

	static inline uint32 RandomI()
	{
		return RandomSeed = GenerateWangHash(RandomSeed);
	}

	static inline float RandomF()
	{
		// RandomI() returns an integer value in range [0, 2^32)
		// Multipltying with 2^-32 maps this to [0, 1)
		return (float)(RandomI() * 2.3283064e-10);
	}

	static inline uint32 RandomI(uint32 MaxI)
	{
		return (uint32)(RandomF() * MaxI);
	}

	static inline float Sin(float EvaluateAt)
	{
		return sinf(EvaluateAt);
	}

	static inline float Cos(float EvaluateAt)
	{
		return cosf(EvaluateAt);
	}

	static inline float Tanh(float EvaluateAt)
	{
		return tanhf(EvaluateAt);
	}

	static inline float InvTanh(float EvaluateAt)
	{
		return atanhf(EvaluateAt);
	}

	static inline float DiffTanh(float EvaluateAt)
	{
		const float ETo2X = PowerNat(2 * EvaluateAt);
		return (4 * ETo2X) / SquareF(ETo2X + 1);
	}

	template <class T>
	static inline void RandomPerm(T* Permutated, int32 Size)
	{
		for (int32 Chosen = Size - 1; Chosen; --Chosen)
		{
			const int32 PermIndex = RandomI(Chosen + 1);
			T Temp = Permutated[PermIndex];
			Permutated[PermIndex] = Permutated[Chosen];
			Permutated[Chosen] = Temp;
		}
	}

	static constexpr float PI = 3.14159265358979323846f;
};

