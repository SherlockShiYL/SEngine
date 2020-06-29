#ifndef INCLUDED_CORE_METAREGISTRY_H
#define INCLUDED_CORE_METAREGISTRY_H

namespace S {
namespace Core {

class MetaClass;

namespace MetaRegistry
{
	void Register(const MetaClass* metaClass);

	const MetaClass* FindMetaClass(const char* className);

} // namespace MetaRegistry

} // namespace Core
} // namespace S

#endif // !INCLUDED_CORE_METAREGISTRY_H