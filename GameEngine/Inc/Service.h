#ifndef INCLUDED_GAMEENGINE_SERVICE_H
#define INCLUDED_GAMEENGINE_SERVICE_H

namespace S {

	class World;

	class Service
	{
	public:
		Service(std::string typeId);
		virtual ~Service();

		virtual void Initialize(){}
		virtual void Terminate(){}

		virtual void Update(float deltaTime){}
		virtual void Render() {}
		virtual void Render2D() {}

		World& GetWorld() { return *mWorld; }
		const World& GetWorld() const { return *mWorld; }

		const std::string& GetTypeId() const { return mtypeId; }

	private:
		friend class World;

		World* mWorld{ nullptr };
		std::string mtypeId;

	};
}

#endif // !INCLUDED_GAMEENGINE_SERVICE_H