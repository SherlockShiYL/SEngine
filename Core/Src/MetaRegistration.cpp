#include "Precompiled.h"
#include "MetaRegistration.h"

#include "MetaRegistry.h"


using namespace S;
using namespace S::Core;

META_TYPE_DEFINE(int, "Int")
META_TYPE_DEFINE(float, "Float")
META_TYPE_DEFINE(bool, "Bool")
META_TYPE_DEFINE(std::string, "String")

void MetaRegistration()
{
#define REGISTER(x) MetaRegistry::Register(x::StaticGetMetaClass())


#undef REGISTER
}