#include "Precompiled.h"
#include "SocketAddress.h"

using namespace S;
using namespace S::Network;

SocketAddress::SocketAddress()
{
	GetAsSockAddrIn()->sin_family = AF_INET;
	GetIP4() = INADDR_ANY;
	GetPort() = 0;
}

SocketAddress::SocketAddress(const char* host, uint16_t port)
{
	GetAsSockAddrIn()->sin_family = AF_INET;
	GetPort() = htons(port);
	inet_pton(AF_INET, host, &GetIP4());
}

SocketAddress::SocketAddress(uint16_t port)
{
	GetAsSockAddrIn()->sin_family = AF_INET;
	GetIP4() = INADDR_ANY;
	GetPort() = htons(port);
}

const char* SocketAddress::GetHost() const
{
	char buffer[INET_ADDRSTRLEN];
	if (!inet_ntop(AF_INET, &(GetAsSockAddrIn()->sin_addr), buffer, INET_ADDRSTRLEN))
	{
		return "";
	}
	return buffer;
}