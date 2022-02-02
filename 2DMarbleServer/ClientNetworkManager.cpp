#include "ClientNetworkManager.h"

using namespace S;

namespace
{
	ClientNetworkManager* sInstance = nullptr;
}

void ClientNetworkManager::StaticInitialize()
{
	ASSERT(sInstance == nullptr, "[ClientNetworkManager] Manager already initialized.");
	sInstance = new ClientNetworkManager();
	sInstance->Initialize();
}

void ClientNetworkManager::StaticTerminate()
{
	ASSERT(sInstance != nullptr, "[ClientNetworkManager] Manager already terminated.");
	sInstance->Terminate();
}

ClientNetworkManager * ClientNetworkManager::Get()
{
	ASSERT(sInstance != nullptr, "[ClientNetworkManager] Manager not initialized.");
	return sInstance;
}

void ClientNetworkManager::Initialize()
{
	Network::Initialize();
}

void ClientNetworkManager::Terminate()
{
	Network::Terminate();
}

bool ClientNetworkManager::ConnectToServer(const char* host, uint16_t port)
{
	return
		mSocket.Open() &&
		mSocket.SetNoDelay(true) &&
		mSocket.SetNonBlocking(true) &&
		mSocket.Connect({ host,port });
}

FILE* ClientNetworkManager::ReceiveFileFromServer()
{
	FILE *ofile;
	fopen_s(&ofile, "tempfile.txt", "wb");

	char cFileSize[1024];
	mSocket.Receive(cFileSize, 1024);
	int fileSize = atoi(cFileSize);

	// receive the file
	char fileBuffer[262148];
	int file_bytesReceived = 0;
	while (file_bytesReceived < fileSize)
	{
		int file_bytesReceivedEachTime = mSocket.Receive(fileBuffer, static_cast<int>(std::size(fileBuffer)));
		printf("file.bytesReceviedEachTime: %d\n", file_bytesReceivedEachTime);
		file_bytesReceived += file_bytesReceivedEachTime;
		printf("file.bytesRecevied: %d\n", file_bytesReceived);
		int bytes_write = static_cast<int>(fwrite(fileBuffer, sizeof(char), file_bytesReceivedEachTime, ofile));
		printf("bytes write: %d\n", bytes_write);
	}

	return ofile;
}

bool ClientNetworkManager::SendMessageToServer(const Network::MemoryStream& memStream)
{
	int bytesSent = mSocket.Send(memStream.GetData(), memStream.GetHead());
	
	char commands[6];
	_itoa_s(bytesSent, commands, 10);
	//X::DrawScreenText(commands, 200.0f, 20.0f, 24.0f, X::Math::Vector4::Magenta());

	return bytesSent != SOCKET_ERROR;
}

bool ClientNetworkManager::HandleMessage(uint8_t *buffer, int size, int &bytesReceived)
{
	bytesReceived = mSocket.Receive(buffer, size);
	if (bytesReceived == SOCKET_ERROR)
		return false;

	return true;
}
