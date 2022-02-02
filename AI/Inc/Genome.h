#ifndef INCLUDED_AI_NEAT_GENOME_H
#define INCLUDED_AI_NEAT_GENOME_H

namespace S {
namespace AI {
namespace NEAT {

struct Gene
{
	size_t innovationNum = -1;
	size_t fromNode = -1;
	size_t toNode = -1;
	double weight{ 0.0 };
	bool enabled{ true };
};

struct Genome
{
	size_t fitness{ 0 };
	size_t adjusteFitness{ 0 };
	size_t globalRank{ 0 };
	size_t maxNeuron{ 0 };

	std::map<size_t, Gene> genes;

	Genome(size_t functional_nodes)
		: maxNeuron{ functional_nodes }
	{
	}
};

} // namespace NEAT
} // namespace AI
} // namespace S

#endif // !INCLUDED_AI_NEAT_GENOME_H