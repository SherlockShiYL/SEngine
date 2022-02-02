#include "Precompiled.h"
#include "InnovationContainer.h"

#include "Genome.h"

using namespace S;
using namespace AI;
using namespace NEAT;

void InnovationContainer::Reset()
{
	mHistory.clear(); 
}

size_t InnovationContainer::AddGene(const Gene& gene)
{
	auto pair = std::make_pair(gene.fromNode, gene.toNode);
	auto it = mHistory.find(pair);
	if (it == mHistory.end())
		return mHistory[pair] = ++mInnovationNumber;
	else
		return (*it).second;
}
