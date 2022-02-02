#include "Precompiled.h"
#include "NeuralNet.h"

#include "Genome.h"
#include <stack>

using namespace S;
using namespace AI;
using namespace NEAT;

namespace
{
	double Sigmoid(double x)
	{
		return 2.0 / (1.0 + std::exp(-4.9*x)) - 1;
	}
}

void NeuralNet::Initialize(const Genome& genome, const NeuralNetConfig& netConfig)
{
	mNodes.clear();
	mInputNodes.clear();
	mBiasNodes.clear();
	mOutputNodes.clear();

	// Add starting node types
	for (size_t i = 0; i < netConfig.input_size; ++i)
	{
		mInputNodes.push_back(mNodes.size());
		mNodes.emplace_back().type = Neuron::Type::Input;
	}
	for (size_t i = 0; i < netConfig.bias_size; ++i)
	{
		mBiasNodes.push_back(mNodes.size());
		mNodes.emplace_back().type = Neuron::Type::Bias;
	}
	for (size_t i = 0; i < netConfig.output_size; ++i)
	{
		mOutputNodes.push_back(mNodes.size());
		mNodes.emplace_back().type = Neuron::Type::Output;
	}

	std::map<size_t, size_t> nodeIndexLookup;
	for (size_t i = 0; i < mInputNodes.size() + mOutputNodes.size() + mBiasNodes.size(); ++i)
		nodeIndexLookup[i] = i;

	// Go through all the connections and add any hidden nodes
	for (auto&[innov, gene] : genome.genes)
	{
		if (!gene.enabled)
			continue;

		if (nodeIndexLookup.find(gene.fromNode) == nodeIndexLookup.end())
		{
			nodeIndexLookup[gene.fromNode] = mNodes.size();
			mNodes.emplace_back().type = Neuron::Type::Hidden;
		}
		if (nodeIndexLookup.find(gene.toNode) == nodeIndexLookup.end())
		{
			nodeIndexLookup[gene.toNode] = mNodes.size();
			mNodes.emplace_back().type = Neuron::Type::Hidden;
		}
	}

	// Connect nodes
	for (auto&[innov, gene] : genome.genes)
	{
		auto toNodeIndex = nodeIndexLookup[gene.toNode];
		auto fromNodeIndex = nodeIndexLookup[gene.fromNode];
		mNodes[toNodeIndex].inNodes.emplace_back(fromNodeIndex, gene.weight);
	}
}

std::vector<double> NeuralNet::Evaluate(const std::vector<double>& input)
{
	// Reset all nodes
	for (auto& node : mNodes)
	{
		node.value = 0.0;
		node.visited = false;
	}

	// Assign values to the input nodes
	ASSERT(input.size() == mInputNodes.size(), "NeuralNet -- Input count mismatch.");
	for (size_t i = 0; i < input.size(); ++i)
	{
		mNodes[mInputNodes[i]].value = input[i];
		mNodes[mInputNodes[i]].visited = true;
	}

	// Assign 1 to all bias nodes
	for (auto biasNodeIndex : mBiasNodes)
	{
		mNodes[biasNodeIndex].value = 1.0;
		mNodes[biasNodeIndex].visited = true;
	}

	// Start evaluating from the output nodes
	std::stack<size_t> s;
	for (auto outputNodeIndex : mOutputNodes)
		s.push(outputNodeIndex);

	// Continue until all nodes are evaluated
	while (!s.empty())
	{
		size_t t = s.top();

		if (mNodes[t].visited)
		{
			double sum{ 0.0 };
			for (auto&[index, weight] : mNodes[t].inNodes)
				sum += mNodes[index].value * weight;
			mNodes[t].value = Sigmoid(sum);
			s.pop();
		}
		else
		{
			mNodes[t].visited = true;

			// Add unvisited parents
			for (auto&[index, weight] : mNodes[t].inNodes)
				if (!mNodes[index].visited)
					s.push(index);
		}
	}

	// Extract results from output nodes
	std::vector<double> outputs;
	for (auto outputNodeIndex : mOutputNodes)
		outputs.push_back(mNodes[outputNodeIndex].value);
	return outputs;
}

void NEAT::NeuralNet::ImportFromFile(const char * filename)
{
	FILE* file = nullptr;
	fopen_s(&file, filename, "r");
	ASSERT(file, "'NeuralNet.cpp' Failed to open file. There is no %s file.", filename);

	mNodes.clear();
	mInputNodes.clear();
	mOutputNodes.clear();

	size_t neuronNumber{ 0 };
	fscanf_s(file, "%zu", &neuronNumber);
	mNodes.resize(neuronNumber);

	for (size_t i = 0; i < neuronNumber; ++i)
	{
		size_t inputSize{ 0 };
		Neuron::Type type;
		mNodes[i].value = 0.0;
		mNodes[i].visited = false;

		fscanf_s(file, "%u", &type);

		switch (type)
		{
		case NEAT::Neuron::Type::Hidden:
			break;
		case NEAT::Neuron::Type::Input:
			mInputNodes.push_back(i);
			break;
		case NEAT::Neuron::Type::Output:
			mOutputNodes.push_back(i);
			break;
		case NEAT::Neuron::Type::Bias:
			mBiasNodes.push_back(i);
			break;
		default:
			break;
		}

		mNodes[i].type = type;

		fscanf_s(file, "%zu", &inputSize);
		for (size_t j = 0; j < inputSize; ++j)
		{
			size_t t{ 0 };
			double w{ 0.0 };
			fscanf_s(file, "%zu %lf", &t, &w);
			mNodes[i].inNodes.push_back(std::make_pair(t, w));
		}
	}

	fclose(file);
}

void NEAT::NeuralNet::ExportToFile(const char * filename)
{
	FILE* file = nullptr;
	fopen_s(&file, filename, "w");

	fprintf(file, "%zu\n", mNodes.size());

	for (auto& n : mNodes)
	{
		fprintf(file, "\t%u %zu\n", n.type, n.inNodes.size());
		for (auto& in : n.inNodes)
			fprintf(file, "\t\t%zu %f\n", in.first, in.second);
	}

	fclose(file);
}
