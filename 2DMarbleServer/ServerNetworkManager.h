#ifndef INCLUDED_SERVERNETWORKMANAGER_H
#define INCLUDED_SERVERNETWORKMANAGER_H

#include "NetworkMessage.h"

enum class NetworkMessage : uint32_t
{
	Snapshot,			// Don't know what this is doing
	Command,
	Refresh,
	Marble
};

class ServerNetworkManager
{
public:
	static void StaticInitialize();
	static void StaticTerminate();
	static ServerNetworkManager* Get();

	void Initialize();
	void Terminate();

	//void Run(uint16_t port);

	bool HandleNewClients(uint32_t& id);
	void ProcessClientMessage(uint8_t *buffer, int size, int &bytesReceived);
	//void BroadcastCreate(GameObject* gameObject);
	void BroadcastWorldUpdate();
	void Broadcast(S::Network::MemoryStream& mem);

	void SendToClient(uint32_t id, S::Network::MemoryStream& mem);

private:
	struct Client
	{
		S::Network::TCPSocket* clientSocket;
		uint32_t networkId;
	};

	std::vector<Client> mClients;
	S::Network::TCPSocket mListener;
};

#endif // !INCLUDED_SERVERNETWORKMANAGER_H