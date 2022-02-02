#include "Precompiled.h"
#include "Config.h"

#include <RapidJSON/Inc/document.h>
#include <RapidJSON/Inc/filereadstream.h>
#include <RapidJSON/Inc/filewritestream.h>
#include <RapidJSON/Inc/prettywriter.h>

using namespace S;
using namespace S::Graphics;

namespace
{
	Config* sConfig = nullptr;
}

class Config::Impl
{
public:
	void Load(const char* fileName);
	void Save();
	void SaveAs(const char* fileName);

	int GetInt(const char* key, int idefault = 0) const;
	bool GetBool(const char* key, bool bdefault = false) const;
	float GetFloat(const char* key, float fdefault = 0.0f) const;
	const char* GetString(const char* key, const char* cdefault = "") const;

	void SetInt(const char* key, int value);
	void SetBool(const char* key, bool value);
	void SetFloat(const char* key, float value);
	void SetString(const char* key, const char* value);

private:
	rapidjson::Document mDocument;
	std::string mFileName;
};

void Config::Impl::Load(const char* fileName)
{
	if (fileName == nullptr)
	{
		LOG("[Config] Not using config file. Default settings will be used.");
		return;
	}

	FILE* file = nullptr;
	fopen_s(&file, fileName, "rb");
	if (file == nullptr)
	{
		LOG("[Config] Failed to open config file %s. Default settings will be used.", fileName);
		return;
	}

	char buffer[65536];
	rapidjson::FileReadStream readStream(file, buffer, std::size(buffer));
	mDocument.ParseStream(readStream);

	fclose(file);

	mFileName = fileName;
}

void Config::Impl::Save()
{
	SaveAs(mFileName.c_str());
}

void Config::Impl::SaveAs(const char* fileName)
{
	FILE* file = nullptr;
	fopen_s(&file, fileName, "wb");
	if (file == nullptr)
	{
		LOG("[Config] Failed to save config file %s.", fileName);
		return;
	}

	char buffer[65536];
	rapidjson::FileWriteStream writeStream(file, buffer, std::size(buffer));
	rapidjson::PrettyWriter<rapidjson::FileWriteStream> writer(writeStream);
	mDocument.Accept(writer);

	fclose(file);
}

int Config::Impl::GetInt(const char* key, int default) const
{
	if (!mDocument.IsObject())
	{
		return default;
	}
	auto iter = mDocument.FindMember(key);
	if (iter != mDocument.MemberEnd() && iter->value.IsInt())
	{
		return iter->value.GetInt();
	}
	return default;
}

bool Config::Impl::GetBool(const char* key, bool default) const
{
	if (!mDocument.IsObject())
	{
		return default;
	}
	auto iter = mDocument.FindMember(key);
	if (iter != mDocument.MemberEnd() && iter->value.IsBool())
	{
		return iter->value.GetBool();
	}
	return default;
}

float Config::Impl::GetFloat(const char* key, float default) const
{
	if (!mDocument.IsObject())
	{
		return default;
	}
	auto iter = mDocument.FindMember(key);
	if (iter != mDocument.MemberEnd() && iter->value.IsFloat())
	{
		return iter->value.GetFloat();
	}
	return default;
}

const char* Config::Impl::GetString(const char* key, const char* default) const
{
	if (!mDocument.IsObject())
	{
		return default;
	}
	auto iter = mDocument.FindMember(key);
	if (iter != mDocument.MemberEnd() && iter->value.IsString())
	{
		return iter->value.GetString();
	}
	return default;
}

void Config::Impl::SetInt(const char* key, int value)
{
	if (!mDocument.IsObject())
	{
		mDocument.StartObject();
	}

	auto iter = mDocument.FindMember(key);
	if (iter != mDocument.MemberEnd())
	{
		ASSERT(iter->value.IsInt(), "[Config] %s is not an integer.", key);
		iter->value.SetInt(value);
	}
	else
	{
		rapidjson::Value k(key, mDocument.GetAllocator());
		mDocument.AddMember(k, value, mDocument.GetAllocator());
	}
}

void Config::Impl::SetBool(const char* key, bool value)
{
	if (!mDocument.IsObject())
	{
		mDocument.StartObject();
	}

	auto iter = mDocument.FindMember(key);
	if (iter != mDocument.MemberEnd())
	{
		ASSERT(iter->value.IsBool(), "[Config] %s is not a boolean.", key);
		iter->value.SetBool(value);
	}
	else
	{
		rapidjson::Value k(key, mDocument.GetAllocator());
		mDocument.AddMember(k, value, mDocument.GetAllocator());
	}
}

void Config::Impl::SetFloat(const char* key, float value)
{
	if (!mDocument.IsObject())
	{
		mDocument.StartObject();
	}

	auto iter = mDocument.FindMember(key);
	if (iter != mDocument.MemberEnd())
	{
		ASSERT(iter->value.IsFloat(), "[Config] %s is not a float.", key);
		iter->value.SetFloat(value);
	}
	else
	{
		rapidjson::Value k(key, mDocument.GetAllocator());
		mDocument.AddMember(k, value, mDocument.GetAllocator());
	}
}

void Config::Impl::SetString(const char* key, const char* value)
{
	if (!mDocument.IsObject())
	{
		mDocument.StartObject();
	}

	rapidjson::Value v(value, mDocument.GetAllocator());

	auto iter = mDocument.FindMember(key);
	if (iter != mDocument.MemberEnd())
	{
		ASSERT(iter->value.IsString(), "[Config] %s is not a boolean.", key);
		iter->value.SetString(value, (rapidjson::SizeType)std::strlen(value));
	}
	else
	{
		rapidjson::Value k(key, mDocument.GetAllocator());
		mDocument.AddMember(k, v, mDocument.GetAllocator());
	}
}

void Config::StaticInitialize(const char* fileName)
{
	ASSERT(sConfig == nullptr, "[Config] System already initialized!");
	sConfig = new Config();
	sConfig->Load(fileName);
}

void Config::StaticTerminate()
{
	if (sConfig != nullptr)
	{
		SafeDelete(sConfig);
	}
}

Config* Config::Get()
{
	ASSERT(sConfig != nullptr, "[Config] No system registered.");
	return sConfig;
}

Config::Config()
	: mImpl(std::make_unique<Impl>())
{
}

void Config::Load(const char* fileName)
{
	mImpl->Load(fileName);
}

void Config::Save()
{
	mImpl->Save();
}

void Config::SaveAs(const char* fileName)
{
	mImpl->SaveAs(fileName);
}

int Config::GetInt(const char* key, int idefault) const
{
	return mImpl->GetInt(key, idefault);
}

bool Config::GetBool(const char* key, bool bdefault) const
{
	return mImpl->GetBool(key, bdefault);
}

float Config::GetFloat(const char* key, float fdefault) const
{
	return mImpl->GetFloat(key, fdefault);
}

const char* Config::GetString(const char* key, const char* cdefault) const
{
	return mImpl->GetString(key, cdefault);
}

void Config::SetInt(const char* key, int value)
{
	mImpl->SetInt(key, value);
}

void Config::SetBool(const char* key, bool value)
{
	mImpl->SetBool(key, value);
}

void Config::SetFloat(const char* key, float value)
{
	mImpl->SetFloat(key, value);
}

void Config::SetString(const char* key, const char* value)
{
	mImpl->SetString(key, value);
}