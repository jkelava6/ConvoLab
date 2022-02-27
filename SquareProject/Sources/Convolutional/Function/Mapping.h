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
	virtual int32 GetUID() = 0;
	virtual bool Equals(IFloatToFloat& Other);
};

class FLinear : public IFloatToFloat
{
public:
	FLinear() = default;
	virtual float EvaluateAt(float X) override;
	virtual float DerivateAt(float X) override;
	virtual int32 GetUID() override { return 0; };
};

class FReLu : public IFloatToFloat
{
public:
	FReLu() = default;
	virtual float EvaluateAt(float X) override;
	virtual float DerivateAt(float X) override;
	virtual int32 GetUID() override { return 1; };
};

class FLeakyReLu : public IFloatToFloat
{
public:
	FLeakyReLu(float InLeak) : Leak(InLeak) {}
	virtual float EvaluateAt(float X) override;
	virtual float DerivateAt(float X) override;
	virtual int32 GetUID() override { return 2; };
	virtual bool Equals(IFloatToFloat& Other) override;
private:
	float Leak;
};

class FTangens : public IFloatToFloat
{
public:
	FTangens() = default;
	virtual float EvaluateAt(float X) override;
	virtual int32 GetUID() override { return 3; };
};

CONVOLUTION_FUNCTION_NAMESPACE_END
