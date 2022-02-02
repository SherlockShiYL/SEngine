#ifndef INCLUDED_AI_NEAT_CONFIG_H
#define INCLUDED_AI_NEAT_CONFIG_H

namespace S {
namespace AI {
namespace NEAT {

struct MutationConfig
{
	void Read(FILE* file)
	{
		fscanf_s(file, "%lf %lf %lf %lf %lf %lf %lf %lf %lf \n",
			&connection_mutate_chance,
			&perturb_chance,
			&crossover_chance,
			&link_mutation_chance,
			&node_mutation_chance,
			&bias_mutation_chance,
			&step_size,
			&disable_mutation_chance,
			&enable_mutation_chance
		);
	}

	void Write(FILE* file)
	{
		fprintf(file, "%f %f %f %f %f %f %f %f %f\n",
			connection_mutate_chance,
			perturb_chance,
			crossover_chance,
			link_mutation_chance,
			node_mutation_chance,
			bias_mutation_chance,
			step_size,
			disable_mutation_chance,
			enable_mutation_chance
		);
	}

	double connection_mutate_chance{ 0.25 };
	double perturb_chance{ 0.9 };
	double crossover_chance{ 0.75 };
	double link_mutation_chance{ 0.2 };
	double node_mutation_chance{ 0.5 };
	double bias_mutation_chance{ 0.4 };
	double step_size{ 0.1 };
	double disable_mutation_chance{ 0.4 };
	double enable_mutation_chance{ 0.2 };
};

struct SpeciatingConfig
{
	void Read(FILE* file)
	{
		fscanf_s(file, "%zu %lf %lf %lf %zu\n",
			&population,
			&delta_disjoint,
			&delta_weights,
			&delta_threshold,
			&stale_species
		);
	}

	void Write(FILE* file)
	{
		fprintf(file, "%zu %f %f %f %zu\n",
			population,
			delta_disjoint,
			delta_weights,
			delta_threshold,
			stale_species
		);
	}

	size_t population{ 240 };
	double delta_disjoint{ 2.0 };
	double delta_weights{ 0.4 };
	double delta_threshold{ 1.3 };
	size_t stale_species{ 100 };
};

struct NeuralNetConfig
{
	size_t input_size{ 0 };
	size_t bias_size{ 0 };
	size_t output_size{ 0 };
	size_t functional_nodes{ 0 };
};

} // namespace NEAT
} // namespace AI
} // namespace S

#endif // !INCLUDED_AI_NEAT_CONFIG_H