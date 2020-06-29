#ifndef INCLUDED_GAMEENGINE_NAVIGATIONSERVICE_H
#define INCLUDED_GAMEENGINE_NAVIGATIONSERVICE_H

#include "Service.h"

namespace S {

struct Graph;

struct PathRequest
{
	Math::Vector3 start;
	Math::Vector3 end;
	std::vector<Math::Vector3> path;
};

class NavigationService : public Service
{
public:
	static std::string StaticGetTypeId() { return "NavigationService"; }
	NavigationService();
	~NavigationService() override;

	void Setup(uint32_t width, uint32_t height, float scale);

	void Initialize() override;
	void Terminate() override;

	void Update(float deltaTime) override;
	void Render() override;
	void Render2D() override;

	void RequestPath(PathRequest& request);

private:
	std::unique_ptr<Graph> mGraph;

	uint32_t mWidth{ 0u };
	uint32_t mHeight{ 0u };
	float mScale{ 0.0f };
};

} // namespace S

#endif // !INCLUDED_GAMEENGINE_NAVIGATIONSERVICE_H