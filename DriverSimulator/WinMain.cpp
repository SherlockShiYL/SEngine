#include "Car.h"
#define DESTINATIONNUM 6

S::TileMap tileMap;
Car car;
std::vector<Car> cars;
std::unique_ptr<S::AI::NEAT::Population> population;
size_t activeCarCount{ 0 };
float totalTime{ 0.0f };
float timePeriod{ 30.0f };
S::Math::Vector2 worldPosition, spawnPosition{ 80.0f,75.0f };
S::Input::InputSystem* input = nullptr;
S::Math::Vector2 destinationList[DESTINATIONNUM];
S::Math::Vector2 destinationList01[DESTINATIONNUM];

bool Init(float deltaTime);
bool Title(float deltaTime);
bool Play(float deltaTime);
bool RunNEAT(float deltaTime);
bool RunXOR(float deltaTime);
void PlayerInput();

std::function<bool(float)> Tick = Init;

bool Init(float deltaTime)
{
	tileMap.Load("../Assets/TileMaps/TileMap02.txt", "../Assets/TileMaps/TileMap_Set.txt");
	Tick = Title;
	input = S::Input::InputSystem::Get();

	destinationList[0] = { 550.0f,141.0f };
	destinationList[1] = { 1220.0f,333.0f };
	destinationList[2] = { 1555.0f,97.0f };
	destinationList[3] = { 1600.0f,600.0f };
	destinationList[4] = { 990.0f,514.0f };
	destinationList[5] = { 160.0f,610.0f };

	/*destinationList01[0] = { 405.0f,184.0f };
	destinationList01[1] = { 431.0f,560.0f };
	destinationList01[2] = { 1000.0f,100.0f };
	destinationList01[3] = { 1350.0f,223.0f };
	destinationList01[4] = { 1470.0f,753.0f };
	destinationList01[5] = { 1640.0f,643.0f };
	destinationList01[6] = { 1592.0f,112.0f };*/


	return input->IsKeyPressed(S::Input::KeyCode::ESCAPE);
}

bool Title(float deltaTime)
{
	tileMap.Render(worldPosition, 1.0f);
	S::Graphics::DrawScreenText("Hit [Space] to Play", 10.0f, 10.0f, 20.0f, S::Math::Vector4::White());
	S::Graphics::DrawScreenText("Hit [N] to NEAT", 10.0f, 30.0f, 20.0f, S::Math::Vector4::White());
	S::Graphics::DrawScreenText("Hit [R] to Resume NEAT", 10.0f, 50.0f, 20.0f, S::Math::Vector4::White());
	S::Graphics::DrawScreenText("Hit [G] to Go Final", 10.0f, 70.0f, 20.0f, S::Math::Vector4::White());
	S::Graphics::DrawScreenText("Hit [X] to XOR Test", 10.0f, 90.0f, 20.0f, S::Math::Vector4::White());


	if (input->IsKeyPressed(S::Input::KeyCode::SPACE))
	{
		car.Load(0.0f, 50.0f, 200.0f, destinationList[0]);
		car.Spawn({ 80.0f,75.0f });
		Tick = Play;
	}
	else if (input->IsKeyPressed(S::Input::KeyCode::N))
	{
		// 25 collision points + 1 speed
		population = std::make_unique<S::AI::NEAT::Population>(MAXCOLLISIONINPUT, 4);

		S::AI::NEAT::MutationConfig& mutationConfig = population->mutationConfig;
		mutationConfig.connection_mutate_chance = 0.65;
		mutationConfig.perturb_chance = 0.9;
		mutationConfig.crossover_chance = 0.75;
		mutationConfig.link_mutation_chance = 0.85;
		mutationConfig.node_mutation_chance = 0.5;
		mutationConfig.bias_mutation_chance = 0.2;
		mutationConfig.step_size = 0.1;
		mutationConfig.disable_mutation_chance = 0.2;
		mutationConfig.enable_mutation_chance = 0.2;

		for (auto& s : population->species)
		{
			for (auto& g : s.genomes)
			{
				auto& c = cars.emplace_back();
				c.Load(0.0f, 50.0f, 200.0f, destinationList[0]);
				c.Spawn(spawnPosition);

				c.brain = std::make_unique<S::AI::NEAT::NeuralNet>();
				c.brain->Initialize(g, population->neuralNetConfig);
				c.fitness = 0;

				++activeCarCount;
			}
		}

		Tick = RunNEAT;
	}
	else if (input->IsKeyPressed(S::Input::KeyCode::R))
	{
		population = std::make_unique<S::AI::NEAT::Population>(MAXCOLLISIONINPUT, 4);

		population->ImportFromFile("TEST.txt");

		// iterator
		auto specie_it = population->species.begin();

		// init initial
		for (auto& s : population->species)
		{
			for (auto& g : s.genomes)
			{
				auto& c = cars.emplace_back();
				c.Load(0.0f, 50.0f, 200.0f, destinationList[0]);
				c.Spawn(spawnPosition);

				c.brain = std::make_unique<S::AI::NEAT::NeuralNet>();
				c.brain->Initialize(g, population->neuralNetConfig);

				c.fitness = 0;

				++activeCarCount;
			}
		}

		Tick = RunNEAT;
	}
	else if (input->IsKeyPressed(S::Input::KeyCode::G))
	{
		population = std::make_unique<S::AI::NEAT::Population>(MAXCOLLISIONINPUT, 4);

		auto& c = cars.emplace_back();
		c.Load(0.0f, 50.0f, 200.0f, destinationList[0]);
		c.Spawn(spawnPosition);

		c.brain = std::make_unique<S::AI::NEAT::NeuralNet>();
		c.brain->ImportFromFile("bestcar.txt");

		c.fitness = 0;

		++activeCarCount;

		Tick = RunNEAT;
	}
	else if (input->IsKeyPressed(S::Input::KeyCode::X))
	{
		Tick = RunXOR;
	}

	return input->IsKeyPressed(S::Input::KeyCode::ESCAPE);
}

bool Play(float deltaTime)
{
	//tileMap.Update(deltaTime);
	tileMap.Render(worldPosition, 1.0f);

	if (input->IsKeyDown(S::Input::KeyCode::A))
	{
		car.TurnLeft();
	}
	if (input->IsKeyDown(S::Input::KeyCode::D))
	{
		car.TurnRight();
	}
	if (input->IsKeyDown(S::Input::KeyCode::W))
	{
		car.Accelerate();
	}
	if (input->IsKeyDown(S::Input::KeyCode::S))
	{
		car.Decelerate();
	}

	PlayerInput();

	car.Update(deltaTime);
	car.CheckMapCollision(tileMap);
	car.Render(worldPosition);

	for (int i = 0; i < DESTINATIONNUM; i++)
	{
		S::Graphics::DrawScreenCircle({ destinationList[i] + worldPosition,40.0f }, S::Math::Vector4::Orange());
	}

	std::string txt;
	txt += std::to_string(input->GetMouseScreenX() - worldPosition.x) + "    " + std::to_string(input->GetMouseScreenY() - worldPosition.y);
	S::Graphics::DrawScreenText(txt.c_str(), static_cast<float>(input->GetMouseScreenX()), static_cast<float>(input->GetMouseScreenY()) - 30.0f, 20.0f, S::Math::Vector4::White());

	return input->IsKeyPressed(S::Input::KeyCode::ESCAPE);
}

bool RunNEAT(float deltaTime)
{
	deltaTime = 0.01f;
	if (activeCarCount == 0)
	{
		// Feed car fitness back into the genome
		for (auto& s : population->species)
			for (auto& g : s.genomes)
				g.fitness = cars[activeCarCount++].fitness;

		population->NewGeneration();

		// Use new species/genomes to respawn cars with new brains
		activeCarCount = 0;
		for (auto& s : population->species)
		{
			for (auto& g : s.genomes)
			{
				auto& c = cars[activeCarCount++];
				c.Spawn(spawnPosition);
				c.Reset();
				c.SetDestination(destinationList[0]);
				c.brain = std::make_unique<S::AI::NEAT::NeuralNet>();
				c.brain->Initialize(g, population->neuralNetConfig);
				c.fitness = 0;
			}
		}

		// Export data per mutation
		population->ExportToFile("TEST.txt");

		totalTime = 0.0f;
	}

	//tileMap.Update(deltaTime);

	activeCarCount = 0;
	totalTime += deltaTime;
	for (auto& c : cars)
	{
		if (c.IsAlive())
		{
			c.Update(deltaTime);
			c.CheckMapCollision(tileMap);
			++activeCarCount;
			if (totalTime > timePeriod)
			{
				activeCarCount = 0;
			}
		}
		if (c.IsArrived())
		{
			c.IncreaseDestinationIndex();
			if (c.GetDestinationIndex() < DESTINATIONNUM)
			{
				c.SetDestination(destinationList[c.GetDestinationIndex()]);
			}
			else if (c.GetDestinationIndex() == DESTINATIONNUM)
			{
				c.SetDestination(0);
				c.ResetDestinationIndex();
			}
			else
			{
				c.brain->ExportToFile("bestcar.txt");
				timePeriod = 30.0f;
				Tick = Init;
			}
		}
	}

	tileMap.Render(worldPosition, 1.0f);
	for (auto& c : cars)
		c.Render(worldPosition);

	std::string txt;
	txt += "Generation: " + std::to_string(population->Generation());
	S::Graphics::DrawScreenText(txt.c_str(), 10.0f, 10.0f, 20.0f, S::Math::Vector4::White());

	txt = "Alive: " + std::to_string(activeCarCount);
	S::Graphics::DrawScreenText(txt.c_str(), 10.0f, 30.0f, 20.0f, S::Math::Vector4::White());

	txt = "Species: " + std::to_string(population->species.size());
	S::Graphics::DrawScreenText(txt.c_str(), 10.0f, 50.0f, 20.0f, S::Math::Vector4::White());

	txt = "DeltaTime: " + std::to_string(deltaTime);
	S::Graphics::DrawScreenText(txt.c_str(), 10.0f, 70.0f, 20.0f, S::Math::Vector4::White());

	txt = "Total Time: " + std::to_string(totalTime);
	S::Graphics::DrawScreenText(txt.c_str(), 10.0f, 90.0f, 20.0f, S::Math::Vector4::White());

	for (int i = 0; i < 10; i++)
	{
		S::Graphics::DrawScreenCircle({ destinationList[i] + worldPosition,40.0f }, S::Math::Vector4::Orange());
	}

	PlayerInput();
	return input->IsKeyPressed(S::Input::KeyCode::ESCAPE);
}

bool RunXOR(float deltaTime)
{
	auto XORTest = [](S::AI::NEAT::NeuralNet& n, bool log) -> size_t
	{
		size_t fitness = 0;

		std::string message = "     > begin xor test\n        ( ";

		auto output = n.Evaluate({ 0.0, 0.0 });
		message += std::to_string(output[0]) + " ";
		fitness += (size_t)std::min(1.0 / ((0.0 - output[0]) * (0.0f - output[0])), 50.0);

		output = n.Evaluate({ 0.0, 1.0 });
		message += std::to_string(output[0]) + " ";
		fitness += (size_t)std::min(1.0 / ((1.0 - output[0]) * (1.0 - output[0])), 50.0);

		output = n.Evaluate({ 1.0, 0.0 });
		message += std::to_string(output[0]) + " ";
		fitness += (size_t)std::min(1.0 / ((1.0 - output[0]) * (1.0 - output[0])), 50.0);

		output = n.Evaluate({ 1.0, 1.0 });
		message += std::to_string(output[0]) + " ";
		fitness += (size_t)std::min(1.0 / ((0.0 - output[0]) * (0.0 - output[0])), 50.0);

		if (log)
		{
			message += ") fitness = " + std::to_string(fitness);
			LOG("%s", message.c_str());
		}

		return fitness;
	};

	S::AI::NEAT::Population p(2, 1);
	S::AI::NEAT::NeuralNet bestGuy;

	S::AI::NEAT::MutationConfig& mutationConfig = p.mutationConfig;
	mutationConfig.connection_mutate_chance = 0.65;
	mutationConfig.perturb_chance = 0.9;
	mutationConfig.crossover_chance = 0.75;
	mutationConfig.link_mutation_chance = 0.85;
	mutationConfig.node_mutation_chance = 0.5;
	mutationConfig.bias_mutation_chance = 0.2;
	mutationConfig.step_size = 0.1;
	mutationConfig.disable_mutation_chance = 0.2;
	mutationConfig.enable_mutation_chance = 0.2;

	size_t max_fitness = 0;
	while (max_fitness < 200)
	{
		size_t current_fitness = 0;
		size_t min_fitness = 100000;
		for (auto& s : p.species)
		{
			for (auto& g : s.genomes)
			{
				S::AI::NEAT::NeuralNet n;
				n.Initialize(g, p.neuralNetConfig);
				current_fitness = XORTest(n, false);
				if (current_fitness < min_fitness)
					min_fitness = current_fitness;
				if (current_fitness > max_fitness)
				{
					max_fitness = current_fitness;
					bestGuy = n;
				}
				g.fitness = current_fitness;
			}
		}

		LOG("Generation %zd successfuly tested. Species: %zd, Global min fitness: %zd, Global max fitness: %zd", p.Generation(), p.species.size(), min_fitness, max_fitness);
		p.NewGeneration();
	}

	XORTest(bestGuy, true);
	return true;
}

void PlayerInput()
{
	if (input->IsMouseDown(S::Input::MouseButton::RBUTTON))
	{
		worldPosition.x += static_cast<float>(input->GetMouseMoveX());
		worldPosition.y += static_cast<float>(input->GetMouseMoveY());
	}
	if (worldPosition.x > 0.0f)
		worldPosition.x = 0.0f;
	if (worldPosition.y > 0.0f)
		worldPosition.y = 0.0f;
}

bool GameLoop(float deltaTime)
{
	return Tick(deltaTime);
}




//bool GameLoop(float deltaTime)
//{
//	ImGui::SliderFloat("left", &rect.left, 0.0f, 100.0f);
//	ImGui::SliderFloat("top", &rect.top, 0.0f, 100.0f);
//	ImGui::SliderFloat("right", &rect.right, 0.0f, 100.0f);
//	ImGui::SliderFloat("bottom", &rect.bottom, 0.0f, 100.0f);
//	ImGui::SliderFloat("from.x", &l.from.x, 0.0f, 100.0f);
//	ImGui::SliderFloat("from.y", &l.from.y, 0.0f, 100.0f);
//	ImGui::SliderFloat("to.x", &l.to.x, 0.0f, 100.0f);
//	ImGui::SliderFloat("to.y", &l.to.y, 0.0f, 100.0f);
//
//	S::Math::Vector2 p = S::Geometry::GetClosestPoint(l, rect);
//	S::Graphics::DrawScreenCircle({ p,3.0f }, S::Math::Vector4::Magenta());
//
//	if (S::Geometry::Intersect(l, rect))
//	{
//		S::Graphics::DrawScreenRect(rect, S::Math::Vector4::Magenta());
//		S::Graphics::DrawScreenLine(l, S::Math::Vector4::Magenta());
//	}
//	else
//	{
//		S::Graphics::DrawScreenRect(rect, S::Math::Vector4::Cyan());
//		S::Graphics::DrawScreenLine(l, S::Math::Vector4::Cyan());
//	}
//	return S::Input::InputSystem::Get()->IsKeyPressed(S::Input::KeyCode::ESCAPE);
//}

int CALLBACK WinMain(HINSTANCE hInstance, HINSTANCE, LPSTR, int)
{
	S::GameApplication myApp;

	myApp.Initialize(hInstance, "sconfig.json");

	while (myApp.IsRunning())
	{
		myApp.Update(GameLoop);
	}

	myApp.Terminate();

	return 0;
}