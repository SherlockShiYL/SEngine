#ifndef INCLUDED_AI_NEAT_POPULATION_H
#define INCLUDED_AI_NEAT_POPULATION_H

#include "Config.h"
#include "Genome.h"
#include "InnovationContainer.h"
#include "Species.h"

namespace S {
namespace AI {
namespace NEAT {

class Population
{
public:
	Population(size_t input, size_t output, size_t bias = 1);

	void NewGeneration();

	size_t Generation() const { return mGenerationNumber; }

	MutationConfig mutationConfig;
	SpeciatingConfig speciatingConfig;
	NeuralNetConfig neuralNetConfig;

	std::list<Species> species;

	// File
	void ImportFromFile(const char * filename);
	void ExportToFile(const char * filename);

private:
	Genome MakeFirstGenome();

	// Evolutionary Methods
	Genome Crossover(const Genome& g1, const Genome& g2);
	void MutateWeight(Genome& g);
	void MutateEnableDisable(Genome& g, bool enable);
	void MutateLink(Genome& g, bool force_bias);
	void MutateNode(Genome& g);
	void Mutate(Genome& g);

	double Disjoint(const Genome& g1, const Genome& g2);
	double Weights(const Genome& g1, const Genome& g2);
	bool IsSameSpecies(const Genome& g1, const Genome& g2);

	// Specie ranking
	void RankGlobally();
	void CalculateAverageFitness(Species& s);
	size_t TotalAverageFitness() const;

	// Evolution
	void CullSpecies(bool cut_to_one);
	Genome BreedChild(const Species& s);
	void RemoveStaleSpecies();
	void RemoveWeakSpecies();
	void AddToSpecies(const Genome& child);

private:
	InnovationContainer mInnovation;

	size_t mGenerationNumber{ 1 };
};

} // namespace NEAT
} // namespace AI
} // namespace S

#endif // !INCLUDED_AI_NEAT_POPULATION_H