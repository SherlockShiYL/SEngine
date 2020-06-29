#include "Precompiled.h"
#include "MetaRegistration.h"

#include "Common.h"

#include "Component.h"
//#include "ColliderComponent.h"
#include "TransformComponent.h"

using namespace S;

META_TYPE_DEFINE(int, "Int")
META_TYPE_DEFINE(float, "Float")
META_TYPE_DEFINE(bool, "Bool")
META_TYPE_DEFINE(std::string, "String")

void S::MetaRegistration()
{
#define REGISTER(x) Core::MetaRegistry::Register(x::StaticGetMetaClass())

	REGISTER(Component);

	//REGISTER(ColliderComponent);
	REGISTER(TransformComponent);

#undef REGISTER
}