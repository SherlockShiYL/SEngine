#include "ServerNetworkManager.h"

using namespace S;

namespace
{
	ServerNetworkManager* sInstance = nullptr;
}

void ServerNetworkManager::StaticInitialize()
{
	ASSERT(sInstance == nullptr, "[ServerNetworkManager] Manager already initialized.");
	sInstance = new ServerNetworkManager();
	sInstance->Initialize();
}

void ServerNetworkManager::StaticTerminate()
{
	ASSERT(sInstance != nullptr, "[ServerNetworkManager] Manager already terminated.");
	sInstance->Terminate();
}

ServerNetworkManager * ServerNetworkManager::Get()
{
	ASSERT(sInstance != nullptr, "[ServerNetworkManager] Manager not initialized.");
	return sInstance;
}

void ServerNetworkManager::Initialize()
{
	Network::Initialize();
	if (!mListener.Open() || !mListener.SetNoDelay(true) || !mListener.SetNonBlocking(true))
	{
		return;
	}

	Network::SocketAddress serverInfo(8888); // Check the port = 8888
	if (!mListener.Bind(serverInfo) || !mListener.Listen())
	{
		return;
	}
}

void ServerNetworkManager::Terminate()
{
	mListener.Close();
	Network::Terminate();
}

//void ServerNetworkManager::Run(uint16_t port)
//{
//	if (!mListener.Open() || !mListener.SetNoDelay(true) || !mListener.SetNonBlocking(true))
//	{
//		return;
//	}
//
//	Network::SocketAddress serverInfo(port);
//	if (!mListener.Bind(serverInfo) || !mListener.Listen())
//	{
//		return;
//	}
//
//	// Accept any incoming client connection
//	HandleNewClients();
//		
//	// Check client message
//	ProcessClientMessage();
//
//	// Update world for 10ms
//	//GameObjectManager::Get()->Update(0.01f);
//
//	// Send update to all clients
//	BroadcastWorldUpdate();
//
//	// Sleep for 10ms
//	std::this_thread::sleep_for(std::chrono::milliseconds(10));
//
//	mListener.Close();
//}

bool ServerNetworkManager::HandleNewClients(uint32_t& id)
{
	Network::SocketAddress address;
	Network::TCPSocket* ClientSocket = mListener.Accept(address);
	if (ClientSocket == nullptr)
	{
		return false; // Nothing to do, just exit
	}

	id = static_cast<uint32_t>(mClients.size());

	// Add new Client
	Client newClient;
	newClient.clientSocket = ClientSocket;
	newClient.networkId = id;
	mClients.push_back(newClient);

	return true;
	//printf("Client joined!\n");

	// Send client a snapshot
	//Network::MemoryStream memStream;
	//Network::StreamWriter writer(memStream);
	//writer.Write(MessageType::Snapshot);
	//GameObjectManager::Get()->Serialize(writer, false);
	//ClientSocket->Send(memStream.GetData(), memStream.GetHead());

	//// Create a new player for this client
	//Character* newCharacter = (Character*)GameObjectManager::Get()->CreateGameObject(Character::ClassId);
	//memStream.Reset();
	//writer.Write(MessageType::Assignment);
	//writer.Write(newCharacter->GetNetworkId());
	//ClientSocket->Send(memStream.GetData(), memStream.GetHead());

	//printf("Assigned client to be network id %d\n", newCharacter->GetNetworkId());
	//
	//// Add new Client
	//Client newClient;
	//newClient.clientSocket = ClientSocket;
	//newClient.networkId = newCharacter->GetNetworkId();
	//mClients.push_back(newClient);

	//// Tell everyone about the new character
	//newCharacter->SetPosition(X::RandomVector2({ 0.0f,0.0f }, { 1280,720 }));
	//BroadcastCreate(newCharacter);
}

void ServerNetworkManager::ProcessClientMessage(uint8_t *buffer, int size, int &bytesReceived)
{
	bytesReceived = 0;
	for (uint32_t i = 0; i < mClients.size(); ++i)
	{
		if (mClients[i].clientSocket == nullptr)
			continue;

		int bytesReceivedEach = mClients[i].clientSocket->Receive(buffer + bytesReceived, size);

		if (bytesReceivedEach > 0)
		{
			bytesReceived += bytesReceivedEach;
		}
		else if (bytesReceivedEach == SOCKET_ERROR)
		{
			mClients[i].clientSocket->Close();
			SafeDelete(mClients[i].clientSocket);

			Network::MemoryStream memStream;
			Network::StreamWriter writer(memStream);
			//writer.Write(NetworkMessage::DestroyPlayer); // Send ID
			//writer.Write(i);
			//for (uint32_t j = 0; j < MAXCLIENTS; ++j)
			//{
			//	if (mClients[j]->clientSocket)
			//	{
			//		mClients[j]->clientSocket->Send(memStream.GetData(), memStream.GetHead());
			//	}
			//}
			//GameObjectManager::Get()->FindPlayer(i)->Kill();
			LOG("Client %u Quit\n", i);
			//printf("No.%u SOKET_ERROR!", i); // Not sure
			// TODO: Delete client character here
		}
	}
	//printf("Bytes Received: %d\n", bytesReceived);
}

void ServerNetworkManager::BroadcastWorldUpdate()
{
	// Serialize world objects
	Network::MemoryStream memStream;
	Network::StreamWriter writer(memStream);
	writer.Write(NetworkMessage::Command);
	//GameObjectManager::Get()->Serialize(writer, true);

	// Check if we have anything to send
	if (memStream.GetHead() <= 4)
		return;

	// Send to all clients
	for (auto& client : mClients)
	{
		if (client.clientSocket != nullptr)
		{
			client.clientSocket->Send(memStream.GetData(), memStream.GetHead());
		}
	}
}

void ServerNetworkManager::Broadcast(Network::MemoryStream& mem)
{
	//int bytesSent = mClients[id]->clientSocket->Send(mem.GetData(), mem.GetHead());
	// Send to all clients
	if (mem.GetData())
	{
		Network::MemoryStream writerStream;
		Network::StreamWriter dataWriter(writerStream);
		for (uint32_t i = 0; i < mClients.size(); ++i)
		{
			if (mClients[i].clientSocket != nullptr)
			{
				mClients[i].clientSocket->Send(writerStream.GetData(), writerStream.GetHead());
				mClients[i].clientSocket->Send(mem.GetData(), mem.GetHead());
			}
		}
	}
}

void ServerNetworkManager::SendToClient(uint32_t id, Network::MemoryStream& memStream)
{
	int bytesSent = mClients[id].clientSocket->Send(memStream.GetData(), memStream.GetHead());
	printf("BytesSent: %d\n", bytesSent);
}