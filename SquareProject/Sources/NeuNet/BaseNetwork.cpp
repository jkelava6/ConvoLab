#include <NeuNet/StdH.h>
#include <NeuNet/BaseNetwork.h>

BASE_NETWORK_NAMESPACE_BEGIN

static constexpr double MinConsideredDifferential = 1e-20;
static constexpr float MaxBiasValue = 2.0f;
static constexpr float MaxEdgeWeight = 4.0f;
static constexpr float MaxValueChange = 0.1f;

FEdge::FEdge() = default;

FEdge::FEdge(int32 InIndex, float InWeight)
	: Index(InIndex)
	, Weight(InWeight)
{
}

FNode::FNode(float InBias)
	: Bias(InBias)
{
}

FNetwork::FNetwork(int32 InInputs, int32 InOutputs)
	: NumOfInputs(InInputs)
	, NumOfOutputs(InOutputs)
{
}

void FNetwork::Evaluate(const TArray<float>& InputData, TArray<float>& OutputData)
{
	for (int32 Input = 0; Input < NumOfInputs; ++Input)
	{
		Neurons[Input].State = InputData[Input];
	}

	const int32 NeuronCount = Neurons.Count();
	for (int32 Neuron = NumOfInputs; Neuron < NeuronCount; ++Neuron)
	{
		EvalNode(Neurons[Neuron]);
	}

	const int32 FirstOutput = NeuronCount - NumOfOutputs;
	OutputData.Clear();
	for (int32 Output = 0; Output < NumOfOutputs; ++Output)
	{
		OutputData.Push() = Neurons[FirstOutput + Output].State;
	}
}

void FNetwork::BackPropagate(const float* OutputErrorDifferentials, bool bHold/* = false*/)
{
	if (bHold && BackPropDiffs == nullptr)
	{
		BackPropDiffs = new FNetwork(NumOfInputs, NumOfOutputs);
		for (int32 Node = 0; Node < GetNeuronCount(); ++Node)
		{
			BackPropDiffs->AddNode(0.0f);
			FNode& Copied = Neurons[Node];
			for (int32 Edge = 0; Edge < Copied.Links.Count(); ++Edge)
			{
				BackPropDiffs->AddEdge(-1, Node, 0.0f);
			}
		}
	}

	TArray<double> ErrorDiffs;
	const int32 NeuronCount = Neurons.Count();
	ErrorDiffs.Prealocate(NeuronCount);

	const int32 FirstOutput = NeuronCount - NumOfOutputs;
	for (int32 Node = 0; Node < FirstOutput; ++Node)
	{
		ErrorDiffs.Push() = 0;
	}
	for (int32 Output = 0; Output < NumOfOutputs; ++Output)
	{
		ErrorDiffs.Push() = OutputErrorDifferentials[Output];
	}

	for (int32 NodeIndex = NeuronCount - 1; NodeIndex >= NumOfInputs; --NodeIndex)
	{
		FNode& Node = Neurons[NodeIndex];

		const float PreTanValue = FMath::InvTanh(Node.State);
		const float SumDifferential = FMath::DiffTanh(PreTanValue) * (float)ErrorDiffs[NodeIndex];

		if (SumDifferential > MinConsideredDifferential)
		{
			const float BiasChange = FMath::ClampF(SumDifferential, -MaxValueChange, MaxValueChange);
			if (bHold)
			{
				BackPropDiffs->Neurons[NodeIndex].Bias -= BiasChange;
			}
			else
			{
				Node.Bias = FMath::ClampF(Node.Bias - BiasChange, -MaxBiasValue, MaxBiasValue);
			}
		}

		const int32 NumOfEdges = Node.Links.Count();
		for (int32 EdgeIndex = 0; EdgeIndex < NumOfEdges; ++EdgeIndex)
		{
			FEdge& Link = Node.Links[EdgeIndex];
			FNode& Feeder = Neurons[Link.Index];

			ErrorDiffs[Link.Index] += Link.Weight * (double)SumDifferential;

			const float EdgeDifferential = Feeder.State * SumDifferential;
			const float WeightChange = FMath::ClampF(EdgeDifferential, -MaxValueChange, MaxValueChange);
			if (bHold)
			{
				BackPropDiffs->Neurons[NodeIndex].Links[EdgeIndex].Weight -= WeightChange;
			}
			else
			{
				Link.Weight = FMath::ClampF(Link.Weight - WeightChange, -MaxEdgeWeight, MaxEdgeWeight);
			}
		}
	}
}

void FNetwork::ResolveBP()
{
	if (!BackPropDiffs)
	{
		return;
	}

	for (int32 NodeIndex = 0; NodeIndex < Neurons.Count(); ++NodeIndex)
	{
		FNode& Node = Neurons[NodeIndex];
		const FNode& DiffNode = BackPropDiffs->Neurons[NodeIndex];
		const float BiasChange = DiffNode.Bias;
		Node.Bias = FMath::ClampF(Node.Bias - BiasChange, -MaxBiasValue, MaxBiasValue);

		for (int32 EdgeIndex = 0; EdgeIndex < Node.Links.Count(); ++EdgeIndex)
		{
			FEdge& Link = Node.Links[EdgeIndex];
			const FEdge& DiffLink = DiffNode.Links[EdgeIndex];
			const float WeightChange = DiffLink.Weight;
			Link.Weight = FMath::ClampF(Link.Weight - WeightChange, -MaxEdgeWeight, MaxEdgeWeight);
		}
	}

	delete BackPropDiffs;
	BackPropDiffs = nullptr;
}

void FNetwork::SetIOCount(int32 InInputs, int32 InOutputs)
{
	NumOfInputs = InInputs;
	NumOfOutputs = InOutputs;
}

int32 FNetwork::AddNode(float Bias)
{
	const int32 AddedIndex = Neurons.Count();
	Neurons.Add(FNode(Bias));
	return AddedIndex;
}

void FNetwork::AddEdge(int32 SourceNode, int32 SinkNode, float Weight)
{
	FNode& Node = Neurons[SinkNode];
	Node.Links.Add(FEdge(SourceNode, Weight));
}

void FNetwork::ChangeNodeBias(int32 NodeIndex, float Bias)
{
	FNode& Node = Neurons[NodeIndex];
	Node.Bias = Bias;
}

void FNetwork::ChangeEdgeWeight(int32 NodeIndex, int32 EdgeIndex, float Weight)
{
	FNode& Node = Neurons[NodeIndex];
	FEdge& Link = Node.Links[EdgeIndex];
	Link.Weight = Weight;
}

const FNode& FNetwork::ReadNode(int32 NodeIndex)
{
	return Neurons[NodeIndex];
}

const FEdge& FNetwork::ReadEdge(int32 NodeIndex, int32 LinkIndex)
{
	return Neurons[NodeIndex].Links[LinkIndex];
}

void FNetwork::EvalNode(FNode& Node)
{
	Node.State = Node.Bias;

	const TArray<FEdge>& Links = Node.Links;
	const int32 InputCount = Links.Count();
	for (int Input = 0; Input < InputCount; ++Input)
	{
		const FEdge Link = Links[Input];
		const FNode& Feeder = Neurons[Link.Index];
		Node.State += Link.Weight * Feeder.State;
	}

	Node.State = FMath::Tanh(Node.State);
}

BASE_NETWORK_NAMESPACE_END
