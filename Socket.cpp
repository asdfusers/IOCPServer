#include "stdafx.h"
#include "Socket.h"
#include "Packet.h"


Socket::Socket() : m_socket(INVALID_SOCKET), recvBytes(0), sendBytes(0), iLevel(0), eStatus(Connect), iRoomNum(0)
{
}


Socket::~Socket()
{
}

void Socket::closeConnection()
{
	shutdown(m_socket, SD_BOTH);
	closesocket(m_socket);
	m_socket = INVALID_SOCKET;


}

void Socket::onReceive(DWORD bytesTransferred)
{
	CPacket receivedPacket;

	recvBytes += bytesTransferred;

	while (recvBytes > 0)
	{
		receivedPacket.copyToBuffer(buffer, recvBytes);

		if (receivedPacket.isValidPacket() == true && recvBytes >= (int)receivedPacket.getPacketSize())
		{
			{
			CCriticalSectionLock Cric(Cric);
			receivedPacket.SetSocketNumber(m_socket);
			recvQue.messageQue.push(receivedPacket);
			}
			unsigned char buffers[PACKETBUFFERSIZE];

			recvBytes -= receivedPacket.getPacketSize();
			if (recvBytes > 0)
			{
				::CopyMemory(buffers, (buffer + receivedPacket.getPacketSize()), recvBytes);
				::CopyMemory(buffer, buffers, recvBytes);
			}
		}
		else
			break;
	}

}
