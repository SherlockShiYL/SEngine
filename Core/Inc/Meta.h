#ifndef INCLUDE_CORE_META_H
#define INCLUDE_CORE_META_H

#include "MetaClass.h"
#include "MetaField.h"
#include "MetaRegistry.h"
#include "MetaType.h"
#include "MetaUtil.h"

#define META_TYPE_DECLARE(Type)\
	template <> const S::Core::MetaType* S::Core::GetMetaType<Type>();

#define META_TYPE_DEFINE(Type, Name)\
	template <> const S::Core::MetaType* S::Core::GetMetaType<Type>()\
	{\
		static S::Core::MetaType sMetaType(\
			Name,\
			S::Core::MetaType::Category::Primitive,\
			sizeof(Type),\
			[] { return new Type; },\
			[](void* data) { delete static_cast<Type*>(data); }\
		);\
	return &sMetaType;\
	}

#define META_CLASS_DECLARE\
	static const S::Core::MetaClass* StaticGetMetaClass();\
	virtual const S::Core::MetaClass* GetMetaClass() const {return StaticGetMetaClass();}

#define META_CLASS_BEGIN_INTERNAL(ClassType)\
	template <> const S::Core::MetaType* S::Core::GetMetaType<ClassType>()\
	{\
		return ClassType::StaticGetMetaClass();\
	}\
	const S::Core::MetaClass* ClassType::StaticGetMetaClass()\
	{\
		using Class = ClassType;\
		const char* const className = #ClassType;

#define META_DERIVED_BEGIN(ClassType, ParentType)\
	META_CLASS_BEGIN_INTERNAL(ClassType)\
		const S::Core::MetaClass* parentMetaClass = ParentType::StaticGetMetaClass();

#define META_CLASS_BEGIN(ClassType)\
	META_CLASS_BEGIN_INTERNAL(ClassType);\
		const S::Core::MetaClass* parentMetaClass = nullptr;

#define META_FIELD_BEGIN\
		static const S::Core::MetaField fields[]={

#define META_FIELD(Var, Name)\
			S::Core::MetaField(Name, S::Core::GetMetaType(&Class::Var), S::Core::GetFieldOffset(&Class::Var)),

#define META_FIELD_END\
		};\
		static const size_t numFields = std::size(fields);

#define META_NO_FIELD\
		static const S::Core::MetaField* fields = nullptr;\
		static const size_t numFields = 0;

#define META_CLASS_END\
		static const S::Core::MetaClass sMetaClass(className, sizeof(Class),\
		[] { return new Class(); },\
		[] (void* data) { delete static_cast<Class*>(data); },\
		parentMetaClass, fields, numFields);\
		return &sMetaClass;\
	}

//#include "Color.h"
//#include "SMath.h"

META_TYPE_DECLARE(int)
META_TYPE_DECLARE(float)
META_TYPE_DECLARE(bool)
META_TYPE_DECLARE(std::string)



#endif // !INCLUDE_CORE_META_H