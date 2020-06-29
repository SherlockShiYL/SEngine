#ifndef INCLUDED_GAMEENGINE_XMLREADER_H
#define INCLUDED_GAMEENGINE_XMLREADER_H

class TiXmlNode;
class MetaClass;

namespace S {

namespace Util
{
	class XMLReader
	{
	public:
		static bool Read(void* instance, const S::Core::MetaClass* metaClass, const TiXmlNode* node);
	};
} // namespace Util
} // namespace S

#endif // !INCLUDED_GAMEENGINE_XMLREADER_H