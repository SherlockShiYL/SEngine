#include "Precompiled.h"
#include "MetaField.h"

using namespace S;
using namespace S::Core;

MetaField::MetaField(const char* const name, const MetaType* const type, size_t offset)
	: mName{ name }
	, mType{ type }
	, mOffset{ offset }
{
}