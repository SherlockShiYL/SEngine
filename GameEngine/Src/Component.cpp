#include "Precompiled.h"
#include "Component.h"
#include "Common.h"

using namespace S;

//META_CLASS_BEGIN(Component)
//META_FIELD_BEGIN
//META_FIELD(mTypeId,"TypeID")
//META_FIELD_END
//META_CLASS_END

Component::Component(std::string typeId)
	: mGameObject{ nullptr }
	, mTypeId{std::move(typeId)}
{
}

Component::~Component()
{
}
