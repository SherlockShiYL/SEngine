#include "Precompiled.h"
#include "UDPSocket.h"

using namespace S;
using namespace S::Network;

UDPSocket::UDPSocket()
	: mSocket(INVALID_SOCKET)
{
}

UDPSocket::~UDPSocket()
{
	Close();
}

bool UDPSocket::Open()
{
	ASSERT(mSocket == INVALID_SOCKET, "[UDPSocket] Socket already opened!");
	mSocket = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
	if (mSocket == INVALID_SOCKET)
	{
		LOG("[UDPSocket] Failed to open socket.");
		return false;
	}
	return true;
}

void UDPSocket::Close()
{
	if (mSocket != INVALID_SOCKET)
	{
		closesocket(mSocket);
		mSocket = INVALID_SOCKET;
	}
}

bool UDPSocket::Bind(const SocketAddress& address)
{
	if (mSocket == INVALID_SOCKET && !Open())
	{
		return false;
	}
	int result = bind(mSocket, &address.mSockAddr, static_cast<int>(address.GetSize()));
	if (result == SOCKET_ERROR)
	{
		LOG("[UDPSocket] Failed to bind socket. Error: %d", WSAGetLastError());
		return false;
	}
	return true;
}

// setsockopt: set socket option
bool UDPSocket::SetBroadcast(bool broadcast)
{
	int iResult = setsockopt(mSocket, SOL_SOCKET, SO_BROADCAST, (char *)&broadcast, broadcast);
	//iResult = setsockopt(mSocket, SOL_SOCKET, SO_REUSEADDR, (char *)&broadcast, broadcast);
	if (iResult == SOCKET_ERROR) 
	{
		LOG("[UDPSocket] Failed to set socket option. Error: %d", WSAGetLastError());
		return true;
	}
	return false;
}

bool UDPSocket::SetNonBlocking(bool nonBlocking)
{
	if (mSocket == INVALID_SOCKET && !Open())
		return false;

	u_long arg = nonBlocking ? 1 : 0;
	int result = ioctlsocket(mSocket, FIONBIO, &arg);
	if (result == SOCKET_ERROR)
	{
		LOG("[UDPSocket] Failed to set non blocking mode. Error: %d", WSAGetLastError());
		Close();
		return false;
	}
	return true;
}

int UDPSocket::SendTo(const void* buffer, int len, const SocketAddress& toAddress)
{
	ASSERT(len > 0, "[UDPSocket] Invalid buffer length.");
	if (mSocket == INVALID_SOCKET && !Open())
	{
		return SOCKET_ERROR;
	}

	int bytesSent = sendto(mSocket, static_cast<const char*>(buffer), len, 0, &toAddress.mSockAddr, (int)toAddress.GetSize());
	if (bytesSent < 0)
	{
		LOG("[UDPSocket] Failed to send data.");
		return SOCKET_ERROR;
	}
	return bytesSent;
}

int UDPSocket::ReceiveFrom(void* buffer, int len, SocketAddress& fromAddress)
{
	int fromLength = (int)fromAddress.GetSize();
	int bytesRead = recvfrom(mSocket, static_cast<char*>(buffer), len, 0, &fromAddress.mSockAddr, &fromLength);
	if (bytesRead >= 0)
	{
		return bytesRead;
	}
	else
	{
		int error = WSAGetLastError();
		if (error == WSAEWOULDBLOCK)
		{
			return 0;
		}
		else if (error == WSAECONNRESET)
		{
			// this can happen if a client close and we haven't DC'd yet.
			// this is the ICMP message being sent back saying the port on that computer is closed
			LOG("[UDPSocket] Connection reset.");
			return -WSAECONNRESET;
		}
		else
		{
			LOG("[UDPSocket] Error on receive.");
			return -error;
		}
	}
}