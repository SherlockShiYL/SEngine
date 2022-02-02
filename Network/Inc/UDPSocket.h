#ifndef INCLUDED_NETWORK_UDPSOCKET_H
#define INCLUDED_NETWORK_UDPSOCKET_H

#include "SocketAddress.h"

namespace S {
namespace Network {

class UDPSocket
{
public:
	UDPSocket();
	~UDPSocket();

	bool Open();
	void Close();

	bool Bind(const SocketAddress& address);
	bool SetBroadcast(bool broadcast);
	bool SetNonBlocking(bool nonBlocking);

	int SendTo(const void* buffer, int len, const SocketAddress& toAddress);
	int ReceiveFrom(void* buffer, int len, SocketAddress& fromAddress);

private:
	UDPSocket(const UDPSocket&) = delete;
	UDPSocket& operator=(const UDPSocket&) = delete;

	SOCKET mSocket;
};

} // namespace Network
} // namespace S

#endif // !INCLUDED_NETWORK_UDPSOCKET_H