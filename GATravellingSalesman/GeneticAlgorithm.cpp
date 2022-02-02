#include "GeneticAlgorithm.h"

#include <random>

using namespace AI;

namespace
{
	inline auto& RandomEngine()
	{
		static std::random_device myRandomDevice{};
		static std::mt19937 myRandomEngine{ myRandomDevice() };
		return myRandomEngine;
	}

	inline int RandomInt(int min, int max)
	{
		return std::uniform_int_distribution<>{ min, max }(RandomEngine());
	}

	inline double RandomFloat()
	{
		return std::uniform_real_distribution<float>{ 0.0f, 1.0f }(RandomEngine());
	}
}

void GeneticAlgorithm::Initialize(int populationSize, const std::vector<S::Math::Vector2>& v, float crossoverRate, float mutationRate, ComputeDistance computeDistance)
{
	if (v.size() < 4)
		return;

	mComputeDistance = std::move(computeDistance);

	// Reset
	mGeneration = 0;
	mCrossoverRate = crossoverRate;
	mMutationRate = mutationRate;
	mFound = false;

	// Create initial population
	for (uint32_t i = 0; i < mPopulation.size(); ++i)
	{
		for (auto& c : mPopulation[i].chromosome)
		{
			delete c;
		}
	}
	mPopulation.clear();
	mPopulation.resize(populationSize);
	mCitiesOrder.clear();
	mCitiesOrder.resize(v.size());
	for (uint32_t i = 0; i < mCitiesOrder.size(); ++i)
	{
		mCitiesOrder[i] = i;
	}
	for (auto& genome : mPopulation)
	{
		genome.chromosome.reserve(v.size());

		for (uint32_t i = 0; i < v.size(); ++i)
		{
			Gene* temp = new Gene{ i, v[i], nullptr, nullptr };
			genome.chromosome.push_back(temp);
		}

		for (uint32_t i = 1; i < v.size(); ++i)
		{
			//std::swap(genome.chromosome[i], genome.chromosome[RandomInt(1, v.size() - 1)]);
			std::swap(mCitiesOrder[i], mCitiesOrder[RandomInt(0, static_cast<int>(mCitiesOrder.size()) - 1)]);
		}

		for (uint32_t i = 0; i + 1 < v.size(); ++i)
		{
			genome.chromosome[mCitiesOrder[i]]->nextNode = genome.chromosome[mCitiesOrder[i + 1]];
			genome.chromosome[mCitiesOrder[i + 1]]->prevNode = genome.chromosome[mCitiesOrder[i]];
		}
		genome.chromosome[mCitiesOrder[v.size() - 1]]->nextNode = genome.chromosome[mCitiesOrder[0]];
		genome.chromosome[mCitiesOrder[0]]->prevNode = genome.chromosome[mCitiesOrder[v.size() - 1]];
	}

	SortPopulation();
}

void GeneticAlgorithm::Advance()
{
	/*if (mFound)
		return;*/

	++mGeneration;

	std::vector<Genome> newGeneration;
	newGeneration.reserve(mPopulation.size());

	// Perform Elitism: keep top 10%, mate the rest using top 50%
	const int eliteCount = (static_cast<int>(mPopulation.size()) * 10) / 100;
	for (int i = 0; i < eliteCount; ++i)
	{
		newGeneration.push_back(mPopulation[i]);
	}

	const int mateCount = (int)mPopulation.size() - eliteCount;
	const int cutoff = static_cast<int>(mPopulation.size()) / 2;
	for (size_t i = 0; i < mateCount; ++i)
	{
		const auto& parent1 = mPopulation[RandomInt(0, cutoff)];
		const auto& parent2 = mPopulation[RandomInt(0, cutoff)];
		newGeneration.push_back(Mate(parent1, parent2));
	}

	for (uint32_t i = eliteCount; i < mPopulation.size(); ++i)
	{
		for (auto& c : mPopulation[i].chromosome)
		{
			delete c;
		}
	}
	mPopulation = std::move(newGeneration);
	SortPopulation();
}

void GeneticAlgorithm::SortPopulation()
{
	for (auto& genome : mPopulation)
	{
		mComputeDistance(genome);
	}
	std::sort(mPopulation.begin(), mPopulation.end(), [](const auto& a, const auto& b) {return a.distance < b.distance; });
}

GeneticAlgorithm::Genome GeneticAlgorithm::Mate(const Genome& parent1, const Genome& parent2)
{
	mOffspring.chromosome.clear();
	uint32_t parentSize = static_cast<uint32_t>(parent1.chromosome.size());
	mOffspring.chromosome.reserve(parentSize);

	for (uint32_t i = 0; i < parentSize; ++i)
	{
		Gene* temp = new Gene{ i, parent1.chromosome[i]->pos, nullptr, nullptr };
		mOffspring.chromosome.push_back(temp);
	}

	for (uint32_t i = 1; i < parentSize; ++i)
	{
		std::swap(mCitiesOrder[i], mCitiesOrder[RandomInt(0, static_cast<int>(mCitiesOrder.size()) - 1)]);
	}

	for (uint32_t i = 0; i + 1 < parentSize; ++i)
	{
		mOffspring.chromosome[mCitiesOrder[i]]->nextNode = mOffspring.chromosome[mCitiesOrder[i + 1]];
		mOffspring.chromosome[mCitiesOrder[i + 1]]->prevNode = mOffspring.chromosome[mCitiesOrder[i]];
	}
	mOffspring.chromosome[mCitiesOrder[parentSize - 1]]->nextNode = mOffspring.chromosome[mCitiesOrder[0]];
	mOffspring.chromosome[mCitiesOrder[0]]->prevNode = mOffspring.chromosome[mCitiesOrder[parentSize - 1]];

	for (uint32_t i = 0; i < parentSize; ++i)
	{
		if (RandomFloat() > mMutationRate)
		{
			if (RandomFloat() < mCrossoverRate)
				SwapWayPoint(mOffspring, parent1, i);
			else
				SwapWayPoint(mOffspring, parent2, i);
		}
	}
	return mOffspring;
}

void AI::GeneticAlgorithm::SwapWayPoint(Genome& w1, const Genome& parent, uint32_t i)
{
	/*{
		XLOG("w1:");
		Gene* temp = w1.chromosome[0];
		do
		{
			XLOG("%u <- %u -> %u", temp->prevNode->id, temp->id, temp->nextNode->id);
			temp = temp->nextNode;
		} while (temp != w1.chromosome[0]);
	}
	{
		XLOG("Parent:");
		Gene* temp = parent.chromosome[0];
		do
		{
			XLOG("%u <- %u -> %u", temp->prevNode->id, temp->id, temp->nextNode->id);
			temp = temp->nextNode;
		} while (temp != parent.chromosome[0]);
		XLOG("============ %u ============", i);
	}*/

	/*{
		XLOG("%u %u %u %u %u %u %u %u %u %u",
			w1.chromosome[0]->id, w1.chromosome[1]->id, w1.chromosome[2]->id, w1.chromosome[3]->id, w1.chromosome[4]->id,
			w1.chromosome[5]->id, w1.chromosome[6]->id, w1.chromosome[7]->id, w1.chromosome[8]->id, w1.chromosome[9]->id);
	}
	{
		XLOG("%u %u %u %u %u %u %u %u %u %u",
			parent.chromosome[0]->id, parent.chromosome[1]->id, parent.chromosome[2]->id, parent.chromosome[3]->id, parent.chromosome[4]->id,
			parent.chromosome[5]->id, parent.chromosome[6]->id, parent.chromosome[7]->id, parent.chromosome[8]->id, parent.chromosome[9]->id);
	}*/
	/*if (w1.chromosome[i]->nextNode->id == parent.chromosome[i]->nextNode->id
		|| w1.chromosome[i]->prevNode->id == parent.chromosome[i]->nextNode->id
		|| w1.chromosome[i]->id == parent.chromosome[i]->nextNode->id)
		return;*/

	Gene* newPrevGene = w1.chromosome[parent.chromosome[i]->nextNode->id]->prevNode;
	Gene* nextGene = w1.chromosome[i]->nextNode;

	w1.chromosome[i]->nextNode = w1.chromosome[parent.chromosome[i]->nextNode->id];
	w1.chromosome[i]->nextNode->prevNode = w1.chromosome[i];
	newPrevGene->nextNode = nextGene;
	newPrevGene->nextNode->prevNode = newPrevGene;
	/*{
		XLOG("First:");
		Gene* temp = w1.chromosome[0];
		do
		{
			XLOG("%u <- %u -> %u", temp->prevNode->id, temp->id, temp->nextNode->id);
			temp = temp->nextNode;
		} while (temp != w1.chromosome[0]);
	}*/
	newPrevGene = newPrevGene->nextNode;
	Gene* newNextGene = w1.chromosome[i]->nextNode->nextNode;
	
	w1.chromosome[i]->nextNode->nextNode = newPrevGene->nextNode;
	w1.chromosome[i]->nextNode->nextNode->prevNode = w1.chromosome[i]->nextNode;
	newPrevGene->nextNode = newNextGene;
	newPrevGene->nextNode->prevNode = newPrevGene;
	/*{
		XLOG("%u %u %u %u %u %u %u %u %u %u",
			w1.chromosome[0]->id, w1.chromosome[1]->id, w1.chromosome[2]->id, w1.chromosome[3]->id, w1.chromosome[4]->id,
			w1.chromosome[5]->id, w1.chromosome[6]->id, w1.chromosome[7]->id, w1.chromosome[8]->id, w1.chromosome[9]->id);
	}*/

	/*{
		XLOG("Second:");
		Gene* temp = w1.chromosome[0];
		do
		{
			XLOG("%u <- %u -> %u", temp->prevNode->id, temp->id, temp->nextNode->id);
			temp = temp->nextNode;
		} while (temp != w1.chromosome[0]);
		XLOG("END");
	}*/
}

/*
namespace
{
	inline auto& RandomEngine()
	{
		static std::random_device myRandomDevice{};
		static std::mt19937 myRandomEngine{ myRandomDevice() };
		return myRandomEngine;
	}

	inline int RandomInt(int min, int max)
	{
		return std::uniform_int_distribution<>{ min, max }(RandomEngine());
	}

	inline double RandomFloat()
	{
		return std::uniform_real_distribution<float>{ 0.0f, 1.0f }(RandomEngine());
	}
}

void GeneticAlgorithm::Initialize(int populationSize, int chromoLength, int maxGeneValue, float crossoverRate, float mutationRate, ComputeFitness computeFitness)
{
	mComputeFitness = std::move(computeFitness);

	// Reset
	mGeneration = 0;
	mMaxGeneValue = maxGeneValue;
	mCrossoverRate = crossoverRate;
	mMutationRate = mutationRate;
	mFound = false;

	// Create initial population
	mPopulation.clear();
	mPopulation.resize(populationSize);
	for (auto& genome : mPopulation)
	{
		genome.chromosome.reserve(chromoLength);
		for (int i = 0; i < chromoLength; i++)
		{
			genome.chromosome.push_back(RandomInt(0, maxGeneValue));
		}
	}
	SortPopulation();
}

void GeneticAlgorithm::Advance()
{
	if (mFound)
		return;

	++mGeneration;

	std::vector<Genome> newGeneration;
	newGeneration.reserve(mPopulation.size());

	// Perform Elitism: keep top 10%, mate the rest using top 50%
	const int eliteCount = (static_cast<int>(mPopulation.size()) * 10) / 100;
	for (int i = 0; i < eliteCount; ++i)
	{
		newGeneration.push_back(mPopulation[i]);
	}

	const int mateCount = (int)mPopulation.size() - eliteCount;
	const int cutoff = static_cast<int>(mPopulation.size()) / 2;
	for (size_t i = 0; i < mateCount; ++i)
	{
		const auto& parent1 = mPopulation[RandomInt(0, cutoff)];
		const auto& parent2 = mPopulation[RandomInt(0, cutoff)];
		newGeneration.push_back(Mate(parent1, parent2));
	}

	mPopulation = std::move(newGeneration);
	SortPopulation();

	if (mPopulation[0].fitness <= 0.0f)
	{
		mFound = true;
	}
}

void GeneticAlgorithm::SortPopulation()
{
	for (auto& genome : mPopulation)
	{
		mComputeFitness(genome);
	}
	std::sort(mPopulation.begin(), mPopulation.end(), [](const auto& a, const auto& b) {return a.fitness < b.fitness; });
}

GeneticAlgorithm::Genome GeneticAlgorithm::Mate(const Genome& parent1, const Genome& parent2)
{
	Genome offspring;
	offspring.chromosome.reserve(parent1.chromosome.size());

	for (size_t i = 0; i < parent1.chromosome.size(); ++i)
	{
		if (RandomFloat() < mMutationRate)
			offspring.chromosome.push_back(RandomInt(0, mMaxGeneValue));
		else if (RandomFloat() < mCrossoverRate)
			offspring.chromosome.push_back(parent1.chromosome[i]);
		else
			offspring.chromosome.push_back(parent2.chromosome[i]);
	}

	return offspring;
}*/