#pragma once

CONVOLUTION_FUNCTION_NAMESPACE_BEGIN

class IFloatToFloat
{
protected:
	IFloatToFloat() = default;
public:
	virtual float EvaluateAt(float X) = 0;
	virtual IFloatToFloat* Derivation();
	virtual float DerivateAt(float X);
};

class FLinear : public IFloatToFloat
{
public:
	FLinear() = default;
	virtual float EvaluateAt(float X) override;
	virtual float DerivateAt(float X) override;
};

class FReLu : public IFloatToFloat
{
public:
	FReLu() = default;
	virtual float EvaluateAt(float X) override;
	virtual float DerivateAt(float X) override;
};

class FLeakyReLu : public IFloatToFloat
{
public:
	FLeakyReLu(float InLeak) : Leak(InLeak) {}
	virtual float EvaluateAt(float X) override;
	virtual float DerivateAt(float X) override;
private:
	float Leak;
};

class FTangens : public IFloatToFloat
{
public:
	FTangens() = default;
	virtual float EvaluateAt(float X) override;
	virtual float DerivateAt(float X) override;
};

CONVOLUTION_FUNCTION_NAMESPACE_END
