#pragma once

#include <GameEngine/Inc/GameEngine.h>

namespace AI
{
	class GeneticAlgorithm
	{
	public:
		struct Gene
		{
			uint32_t id{ 0 };
			S::Math::Vector2 pos{ 0.0f,0.0f };
			Gene* prevNode{ nullptr };
			Gene* nextNode{ nullptr };
		};
		struct Genome
		{
			std::vector<Gene*> chromosome; // [gene][gene][gene][gene] ...
			float distance{ 0.0f };
		};

		using ComputeDistance = std::function<void(Genome&)>;

		// Randomly generate the initial population
		void Initialize(int populationSize, const std::vector<S::Math::Vector2>& v,
			float crossoverRate, float mutationRate, ComputeDistance computeDistance);

		// Apply crossover and mutation to produce the next generation
		void Advance();

		// Accessors
		const Genome& BestGenome() const { return mPopulation.front(); }
		int GetGeneration() const { return mGeneration; }
		bool Found() const { return mFound; }

	private:
		void SortPopulation();
		Genome Mate(const Genome& parent1, const Genome& parent2);
		void SwapWayPoint(Genome& w1, const Genome& parent, uint32_t i);

	private:
		ComputeDistance mComputeDistance;

		Genome mOffspring;
		std::vector<Genome> mPopulation;
		std::vector<uint32_t> mCitiesOrder;
		int mGeneration{ 0 };

		float mCrossoverRate{ 0.0f };
		float mMutationRate{ 0.0f };

		bool mFound{ false };

	};
	/*class GeneticAlgorithm
	{
	public:
		struct Genome
		{
			std::vector<int> chromosome; // [gene][gene][gene][gene] ...
			float fitness{ 0.0f };
		};

		using ComputeFitness = std::function<void(Genome&)>;

		// Randomly generate the initial population
		void Initialize(int populationSize, int chromoLength, int maxGeneValue,
			float crossoverRate, float mutationRate, ComputeFitness computeFitness);

		// Apply crossover and mutation to produce the next generation
		void Advance();

		// Accessors
		const Genome& BestGenome() const	{ return mPopulation.front(); }
		int GetGeneration() const			{ return mGeneration; }
		bool Found() const					{ return mFound; }

	private:
		void SortPopulation();
		Genome Mate(const Genome& parent1, const Genome& parent2);

	private:
		ComputeFitness mComputeFitness;

		std::vector<Genome> mPopulation;
		int mGeneration{ 0 };
		int mMaxGeneValue{ 0 };

		float mCrossoverRate{ 0.0f };
		float mMutationRate{ 0.0f };

		bool mFound{ false };

	};*/
}