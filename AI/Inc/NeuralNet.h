#ifndef INCLUDED_AI_NEAT_NEURALNET_H
#define INCLUDED_AI_NEAT_NEURALNET_H

#include "Config.h"

namespace S {
namespace AI {
namespace NEAT {

struct Genome;
struct Neuron
{
	enum class Type
	{
		Hidden,
		Input,
		Output,
		Bias
	};

	std::vector<std::pair<size_t, double>> inNodes; // Node index and weight pair
	Type type = Type::Hidden;
	double value{ 0.0 };
	bool visited{ false };
};

class NeuralNet
{
public:
	void Initialize(const Genome& genome, const NeuralNetConfig& netConfig);

	std::vector<double> Evaluate(const std::vector<double>& input);

	void ImportFromFile(const char * filename);
	void ExportToFile(const char * filename);

private:
	std::vector<Neuron> mNodes;

	// Indices to the neuron
	std::vector<size_t> mInputNodes;
	std::vector<size_t> mOutputNodes;
	std::vector<size_t> mBiasNodes;
};

} // namespace NEAT
} // namespace AI
} // namespace S

#endif // !INCLUDED_AI_NEAT_NEURALNET_H