#include "Precompiled.h"
#include "NetworkUtil.h"

using namespace S;

void Network::Initialize()
{
	// Initialize Winsock version 2.2
	WSAData wsaData;
	int result = WSAStartup(MAKEWORD(2, 2), &wsaData);
}

void Network::Terminate()
{
	// Shutdown Winsock
	WSACleanup();
}