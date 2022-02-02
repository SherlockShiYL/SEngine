#ifndef INCLUDED_NETWORK_LINKINGCONTEXT_H
#define INCLUDED_NETWORK_LINKINGCONTEXT_H

namespace S {
namespace Network {

class LinkingContext
{
public:
	LinkingContext() = default;

	int GetNetworkId(void* instance) const;
	void* GetInstance(int networkId) const;

	// Register an instance to the next network id
	int Register(void* instance);
	// Register an instance to a specific network id
	void Register(void* instance, int networkId);
	// Unregister an instance
	void Unregister(void* instance);

public:
	static const int sInvalidId = -1;

private:
	std::unordered_map<int, void*> mNetworkIdToInstanceMap;
	std::unordered_map<void*, int> mInstanceToNetworkIdMap;
	int mNextNetworkId{ 0 };
};

} // namespace Network
} // namespace S

#endif // !INCLUDED_NETWORK_LINKINGCONTEXT_H