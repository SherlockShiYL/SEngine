#ifndef INCLUDED_CLIENTNETWORKMANAGER_H
#define INCLUDED_CLIENTNETWORKMANAGER_H

#include <GameEngine/Inc/GameEngine.h>

class ClientNetworkManager
{
public:
	static void StaticInitialize();
	static void StaticTerminate();
	static ClientNetworkManager* Get();

public:
	void Initialize();
	void Terminate();

	bool ConnectToServer(const char* host, uint16_t port);

	FILE* ReceiveFileFromServer();

	bool SendMessageToServer(const S::Network::MemoryStream& memStream);
	bool HandleMessage(uint8_t *buffer, int size, int &bytesReceived);

	uint32_t GetClientId() const { return mClientId; }

private:
	S::Network::TCPSocket mSocket;
	uint32_t mClientId;
};

#endif // !INCLUDED_CLIENTNETWORKMANAGER_H