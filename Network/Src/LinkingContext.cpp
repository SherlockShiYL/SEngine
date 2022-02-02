#include "Precompiled.h"
#include "LinkingContext.h"

using namespace S;
using namespace S::Network;

int LinkingContext::GetNetworkId(void * instance) const
{
	if (mInstanceToNetworkIdMap.empty())
	{
		return sInvalidId;
	}
	for (auto i = mInstanceToNetworkIdMap.begin(); i != mInstanceToNetworkIdMap.end(); ++i)
	{
		if (i->first == instance)
		{
			return i->second;
		}
	}	
	return sInvalidId;
}

void* LinkingContext::GetInstance(int networkId) const
{
	if (mNetworkIdToInstanceMap.empty())
	{
		return nullptr;
	}
	for (auto i = mNetworkIdToInstanceMap.begin(); i != mNetworkIdToInstanceMap.end(); ++i)
	{
		if (i->first == networkId)
		{
			return i->second;
		}
	}
	return nullptr;
}

int LinkingContext::Register(void * instance)
{
	mInstanceToNetworkIdMap.try_emplace(instance, mNextNetworkId);
	mNetworkIdToInstanceMap.try_emplace(mNextNetworkId, instance);
	++mNextNetworkId;
	return mNextNetworkId - 1;
}

void LinkingContext::Register(void * instance, int networkId)
{
	if (networkId > mNextNetworkId)
	{
		mInstanceToNetworkIdMap.try_emplace(instance, mNextNetworkId);
		mNetworkIdToInstanceMap.try_emplace(mNextNetworkId, instance);
	}
	else
	{
		mInstanceToNetworkIdMap.try_emplace(instance, networkId);
		mNetworkIdToInstanceMap.try_emplace(networkId, instance);
	}
	++mNextNetworkId;
}

void LinkingContext::Unregister(void * instance)
{
	int mIndex{ 0 };
	for (auto it = mInstanceToNetworkIdMap.begin(); it != mInstanceToNetworkIdMap.end(); ++it)
	{
		if (it->first == instance)
		{
			mIndex = it->second;
			it = mInstanceToNetworkIdMap.erase(it);
			break;
		}
	}
	mNetworkIdToInstanceMap.erase(mIndex);
}