#ifndef INCLUDED_GAMEENGINE_TRANSFORMCOMPONENT_H
#define INCLUDED_GAMEENGINE_TRANSFORMCOMPONENT_H

#include "Component.h"

namespace S {

	class TransformComponent : public Component
	{
	public:
		META_CLASS_DECLARE;

		static std::string StaticGetTypedId() { return "TransformComponentComponent"; }
		TransformComponent();

		void Initialize() override;
		void Terminate() override;

		void Update(float deltatTime) override;
		void Render() override;
		void Render2D() override;

		void SetPosition(const Math::Vector3& p)	{ mPosition = p; }
		void SetRotation(const Math::Vector3& r)	{ mRotation = r; }
		void SetScale(const Math::Vector3& s)		{ mScale = s; }
		const Math::Vector3 GetPosition() const		{ return mPosition; }
		const Math::Vector3 GetRotation() const		{ return mRotation; }
		const Math::Vector3 GetScale() const		{ return mScale; }

	private:
		Math::Vector3 mPosition;
		Math::Vector3 mRotation;
		Math::Vector3 mScale;

	};
}

#endif // !INCLUDED_GAMEENGINE_TRANSFORMCOMPONENT_H