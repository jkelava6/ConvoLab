#pragma once

#include <Core/DynamicArray.h>

BASE_NETWORK_NAMESPACE_BEGIN

class FEdge
{
public:
	FEdge();
	FEdge(int32 InIndex, float InWeight);
public:
	int32 Index = -1;
	float Weight = 0.0f;
};

class FNode
{
public:
	FNode(float InBias);
public:
	float Bias = 0.0f;
	float State = 0.0f;
	TArray<FEdge> Links;
};

class FNetwork
{
public:
	FNetwork(int32 InInputs, int32 InOutputs);

public:
	void Evaluate(const TArray<float>& InputData, TArray<float>& OutputData);
	void BackPropagate(const float* ErrorDifferentials, bool bHold = false);
	void ResolveBP();

	void SetIOCount(int32 InInputs, int32 InOutputs);
	int32 AddNode(float Bias);
	void AddEdge(int32 SourceNode, int32 SinkNode, float Weight);
	void ChangeNodeBias(int32 NodeIndex, float Bias);
	void ChangeEdgeWeight(int32 NodeIndex, int32 EdgeIndex, float Weight);

	const FNode& ReadNode(int32 NodeIndex);
	const FEdge& ReadEdge(int32 NodeIndex, int32 LinkIndex);
	inline int32 GetNeuronCount() { return Neurons.Count(); }
private:
	void EvalNode(FNode& Node);
private:
	int32 NumOfInputs = -1;
	int32 NumOfOutputs = -1;
	TArray<FNode> Neurons;

	FNetwork* BackPropDiffs = nullptr;
};

BASE_NETWORK_NAMESPACE_END
