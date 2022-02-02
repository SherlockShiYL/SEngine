#ifndef INCLUDED_AI_NEAT_SPECIES_H
#define INCLUDED_AI_NEAT_SPECIES_H

#include "Genome.h"

namespace S {
namespace AI {
namespace NEAT {

struct Species
{
	size_t top_fitness{ 0 };
	size_t average_fitness{ 0 };
	size_t staleness{ 0 };

	std::vector<Genome> genomes;
};

} // namespace NEAT
} // namespace AI
} // namespace S

#endif // !INCLUDED_AI_NEAT_SPECIES_H