#include "Precompiled.h"
#include "Service.h"

using namespace S;

Service::Service(std::string typeId)
	:mtypeId(std::move(typeId))
{}