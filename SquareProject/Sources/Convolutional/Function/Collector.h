#pragma once

CONVOLUTION_FUNCTION_NAMESPACE_BEGIN

class ICollector
{
protected:
	ICollector() = default;
public:
	virtual void Init() = 0;
	virtual void Collect(float X) = 0;
	virtual float Finalize() = 0;

	virtual ICollector* Clone() const = 0;
	virtual float Derivate(float Input) const = 0;
};

class FSum : public ICollector
{
public:
	FSum() = default;
public:
	virtual void Init() override;
	virtual void Collect(float X) override;
	virtual float Finalize() override;

	virtual ICollector* Clone() const override;
	virtual float Derivate(float Input) const override;
private:
	float Sum;
};

class FAverage : public ICollector
{
public:
	FAverage() = default;
public:
	virtual void Init() override;
	virtual void Collect(float X) override;
	virtual float Finalize() override;

	virtual ICollector* Clone() const override;
	virtual float Derivate(float Input) const override;
private:
	float Sum;
	int Count;
};

CONVOLUTION_FUNCTION_NAMESPACE_END
