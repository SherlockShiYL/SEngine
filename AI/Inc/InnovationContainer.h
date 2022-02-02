#ifndef INCLUDED_AI_NEAT_INNOVATIONCONTAINER_H
#define INCLUDED_AI_NEAT_INNOVATIONCONTAINER_H

namespace S {
namespace AI {
namespace NEAT {

struct Gene;

class InnovationContainer
{
public:
	void Reset();

	size_t AddGene(const Gene& g);
	size_t Number() const { return mInnovationNumber; }

private:
	friend class Population;

	void SetInnovationNumber(size_t num)
	{
		mInnovationNumber = num;
		Reset();
	}

	std::map<std::pair<size_t, size_t>, size_t> mHistory;
	size_t mInnovationNumber{ 0 };
};

} // namespace NEAT
} // namespace AI
} // namespace S

#endif // !INCLUDED_AI_NEAT_INNOVATIONCONTAINER_H