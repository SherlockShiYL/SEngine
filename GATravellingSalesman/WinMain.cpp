#include "GeneticAlgorithm.h"
#include <External/ImGui/Inc/imgui.h>

struct City
{
	uint32_t current{ 0 };
	uint32_t next{ 0 };
};

AI::GeneticAlgorithm ga;
int gNumCities{ 40 };
float gCitySize{ 20.0f };
std::vector<S::Math::Vector2> gCitiesPos;
std::vector<City> gCitiesOrder;
S::Math::Vector2 gCenter{ 850.0f,360.0f };
float gRad{ 0.0f };
float gShortestDis{ 1000000000.0f };
float gLastDis{ 1000000000.0f };
uint32_t gGenerationTimes{ 0 };
int gNumChosen{ -1 };
float gDistance{ 0.0f };
bool gRestart{ true };
bool gRandomPos{ false };
bool gQuit{ false };
bool gSameCities{ false };
bool gShowCustomer{ true };
bool gShowGeneration{ true };

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

void ChangeOrder()
{
	int temp{ gNumChosen };
	if (S::Input::InputSystem::Get()->IsMousePressed(S::Input::MouseButton::LBUTTON))
	{
		gNumChosen = -1;
		for (uint32_t i = 0; i < gCitiesOrder.size(); ++i)
		{
			if (S::Math::DistanceSqr(gCitiesPos[gCitiesOrder[i].current], { static_cast<float>(S::Input::InputSystem::Get()->GetMouseScreenX()),static_cast<float>(S::Input::InputSystem::Get()->GetMouseScreenY()) }) < static_cast<float>(gCitySize*gCitySize))
			{
				gNumChosen = gCitiesOrder[i].current;
				break;
			}
		}
	}

	if (gNumChosen != -1 && temp != gNumChosen && temp != -1)
	{
		gCitiesOrder[temp].next = gNumChosen;
		gNumChosen = -1;
		gDistance = 0.0f;
		for (uint32_t i = 0; i < gCitiesPos.size(); ++i)
		{
			gDistance += S::Math::DistanceSqr(gCitiesPos[gCitiesOrder[i].current], gCitiesPos[gCitiesOrder[i].next]);
		}
	}
}

bool GameLoop(float deltaTime)
{
	if (gRestart)
	{
		gShortestDis = 1000000000;
		gLastDis = 10000000000;
		if (gRandomPos)
		{
			if (!gSameCities)
			{
				gCitiesPos.clear();
				gCitiesPos.reserve(gNumCities);
				for (int i = 0; i < gNumCities; ++i)
				{
					bool tooClose{ false };
					do
					{
						tooClose = false;
						S::Math::Vector2 p{ static_cast<float>(RandomFloat())*760.0f + 480.0f,static_cast<float>(RandomFloat())*580.0f + 70.0f };
						float test = 0.0f;
						for (auto& c : gCitiesPos)
						{
							test = S::Math::DistanceSqr(p, c);
							if (S::Math::DistanceSqr(p, c) < gCitySize*gCitySize * 8)
							{
								tooClose = true;
								break;
							}
						}
						if (!tooClose)
						{
							gCitiesPos.push_back(p);
							break;
						}
					} while (true);
				}
				gCitiesOrder.clear();
				for (uint32_t i = 0; i + 1 < gCitiesPos.size(); ++i)
				{
					gCitiesOrder.push_back({ i,i + 1 });
				}
				gCitiesOrder.push_back({ static_cast<uint32_t>(gCitiesPos.size() - 1),0 });
			}
		}
		else
		{
			gCitiesPos.clear();
			gCitiesPos.reserve(gNumCities);
			S::Math::Vector2 direction{ 0.0f,-1.0f };
			if (gNumCities)
				gRad = S::Math::kTwoPi / gNumCities;
			for (int i = 0; i < gNumCities; ++i)
			{
				direction = S::Math::Rotate(direction, gRad);
				gCitiesPos.push_back({ gCenter.x + direction.x*280.0f,gCenter.y + direction.y*280.0f });
			}
			gCitiesOrder.clear();
			for (uint32_t i = 0; i + 1 < gCitiesPos.size(); ++i)
			{
				gCitiesOrder.push_back({ i,i + 1 });
			}
			gCitiesOrder.push_back({ static_cast<uint32_t>(gCitiesPos.size() - 1),0 });
		}

		gDistance = 0.0f;
		for (uint32_t i = 0; i < gCitiesPos.size(); ++i)
		{
			gDistance += S::Math::DistanceSqr(gCitiesPos[gCitiesOrder[i].current], gCitiesPos[gCitiesOrder[i].next]);
		}

		gRestart = false;
		ga.Initialize(2500, gCitiesPos, 0.45f, 0.1f, [](auto& genome)
		{
			genome.distance = 0.0f;
			for (size_t i = 0; i < gCitiesPos.size(); ++i)
			{
				genome.distance += S::Math::DistanceSqr(genome.chromosome[i]->pos, genome.chromosome[i]->nextNode->pos);
			}
		});
	}
	ga.Advance();

	ChangeOrder();

	// Render
	{
		if (gShowGeneration)
		{
			AI::GeneticAlgorithm::Gene* temp = ga.BestGenome().chromosome[0];
			do
			{
				S::Graphics::DrawScreenLine(temp->pos, temp->nextNode->pos, S::Math::Vector4::Cyan());
				temp = temp->nextNode;
			} while (temp != ga.BestGenome().chromosome[0]);
		}
		if (gShowCustomer)
		{
			if (gCitiesOrder.size() > 3)
			{
				for (uint32_t i = 0; i < gCitiesOrder.size(); ++i)
				{
					S::Graphics::DrawScreenLine(gCitiesPos[gCitiesOrder[i].current] + S::Math::Vector2{ 2.0f,2.0f }, gCitiesPos[gCitiesOrder[i].next] + S::Math::Vector2{ 2.0f,2.0f }, S::Math::Vector4::Red());
					S::Graphics::DrawScreenLine(gCitiesPos[gCitiesOrder[i].current] + S::Math::Vector2{ 2.0f,2.0f },
						gCitiesPos[gCitiesOrder[i].current] + S::Math::Normalize(gCitiesPos[gCitiesOrder[i].next] - gCitiesPos[gCitiesOrder[i].current])*30.0f + S::Math::Vector2{ 2.0f,2.0f },
						S::Math::Vector4::White());
				}
			}
		}

		for (auto& c : gCitiesPos)
		{
			S::Graphics::DrawScreenCircle(c, gCitySize, S::Math::Vector4::Blue());
		}
		if (gNumChosen != -1)
		{
			S::Graphics::DrawScreenCircle(gCitiesPos[gNumChosen], gCitySize, S::Math::Vector4::White());
		}

		char labels[10];
		_itoa_s(S::Math::Sqrt(gDistance), labels, 10);
		S::Graphics::DrawScreenText("Customer Distance", 10.0f, 450.0f, 24.0f, S::Math::Vector4::Red());
		S::Graphics::DrawScreenText(labels, 320.0f, 450.0f, 24.0f, S::Math::Vector4::Red());

		_itoa_s(ga.GetGeneration(), labels, 10);
		S::Graphics::DrawScreenText("Generation:", 10.0f, 500.0f, 24.0f, S::Math::Vector4::Cyan());
		S::Graphics::DrawScreenText(labels, 320.0f, 500.0f, 24.0f, S::Math::Vector4::Cyan());
		_itoa_s(S::Math::Sqrt(gLastDis), labels, 10);
		S::Graphics::DrawScreenText("Previous Distance:", 10.0f, 550.0f, 24.0f, S::Math::Vector4::Cyan());
		S::Graphics::DrawScreenText(labels, 320.0f, 550.0f, 24.0f, S::Math::Vector4::Cyan());

		if (gShortestDis > ga.BestGenome().distance)
		{
			if (gLastDis != gShortestDis)
			{
				gLastDis = gShortestDis;
			}
			gShortestDis = ga.BestGenome().distance;
			gGenerationTimes = ga.GetGeneration();
		}
		_itoa_s(gGenerationTimes, labels, 10);
		S::Graphics::DrawScreenText("Generation Time:", 10.0f, 600.0f, 30.0f, S::Math::Vector4::Red());
		S::Graphics::DrawScreenText(labels, 320.0f, 600.0f, 30.0f, S::Math::Vector4::Red());
		_itoa_s(S::Math::Sqrt(gShortestDis), labels, 10);
		S::Graphics::DrawScreenText("Shortest Distance:", 10.0f, 650.0f, 30.0f, S::Math::Vector4::Red());
		S::Graphics::DrawScreenText(labels, 320.0f, 650.0f, 30.0f, S::Math::Vector4::Red());
	}

	// ImGui
	{
		ImGui::SetNextWindowPos({ 30.0f,30.0f });
		ImGui::SliderInt("Number of Cities", &gNumCities, 4, 40);
		if (ImGui::Button("Restart", { 70.0f,50.0f }))
			gRestart = true;
		ImGui::Checkbox("Same Cities", &gSameCities);
		ImGui::Checkbox("Random Cities", &gRandomPos);
		ImGui::Checkbox("Customer's Line", &gShowCustomer);
		ImGui::Checkbox("Generation's Line", &gShowGeneration);
		if (ImGui::Button("Quit", { 70.0f,50.0f }))
			gQuit= true;
	}

	return gQuit || S::Input::InputSystem::Get()->IsKeyPressed(S::Input::KeyCode::ESCAPE);
}

int CALLBACK WinMain(HINSTANCE hInstance, HINSTANCE, LPSTR, int)
{
	S::GameApplication myApp;

	myApp.Initialize(hInstance, "sconfig.json");

	while (myApp.IsRunning())
	{
		myApp.Update(GameLoop);
	}

	myApp.Terminate();

	/*const std::string kTarget = "I love Genetic Algorithm!";
	const int validGeneValue = 126 - 32;

	AI::GeneticAlgorithm ga;
	ga.Initialize(100, (int)kTarget.size(), validGeneValue, 0.45f, 0.1f, [&kTarget](auto& genome)
	{
		genome.fitness = 0.0f;
		for (size_t i = 0; i < kTarget.size(); ++i)
		{
			if (kTarget[i] != genome.chromosome[i] + 32)
				genome.fitness += 1.0f; // penalize any characters that don't match the target
		}
	});

	auto Print = [](const AI::GeneticAlgorithm& ga)
	{
		int gen = ga.GetGeneration();
		auto& fittest = ga.BestGenome();
		std::string result;
		for (auto i : fittest.chromosome)
			result += (char)(i + 32);
		XLOG("Generation: %d - Result: %s - Fitness: %f", gen, result.c_str(), fittest.fitness);
	};

	while (true)
	{
		ga.Advance();
		if (ga.Found())
			break;
		Print(ga);
	}
	Print(ga);*/
	return 0;
}