#ifndef INCLUDED_NETWORK_TCPSOCKET_H
#define INCLUDED_NETWORK_TCPSOCKET_H

#include "SocketAddress.h"

namespace S {
namespace Network {

class TCPSocket
{
public:
	TCPSocket();
	~TCPSocket();

	bool Open();
	void Close();
	bool SetNoDelay(bool noDelay);
	bool SetNonBlocking(bool nonBlocking);

	bool Connect(const SocketAddress& address);
	bool Bind(const SocketAddress& address);
	bool Listen(int backLog = 10);
	TCPSocket* Accept(SocketAddress& fromAddress);

	int Send(const void* buffer, int len, int flags = 0);
	int Receive(void* buffer, int len, int flags = 0);

private:
	TCPSocket(const TCPSocket&) = delete;
	TCPSocket& operator=(const TCPSocket&) = delete;

	SOCKET mSocket;
};

} // namespace Network
} // namespace S
#endif // !INCLUDED_NETWORK_TCPSOCKET_H