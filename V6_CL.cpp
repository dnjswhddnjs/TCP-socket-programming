#include <iostream>
#include <cstring>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <stdlib.h>
#include <arpa/inet.h>

int main()
{
	
	const char* serverIpAddr = "0::1";
	enum ePort { PORT = 54000 };

	
	int sock = socket(AF_INET6, SOCK_STREAM, 0);
	if (sock == -1)
	{
		std::cout << "Can't create socket" << std::endl;
		return -1;
	}

	
	sockaddr_in6 hint;
	hint.sin6_family = AF_INET6;
	hint.sin6_flowinfo = 0;
	hint.sin6_port = htons(PORT);

	int convResult = inet_pton(AF_INET6, serverIpAddr, &hint.sin6_addr);
	if (convResult != 1)
	{
		std::cout << "Can't convert IP address, Err #" << convResult << std::endl;
		
		return -1;
	}

	
	int connResult = connect(sock, reinterpret_cast<sockaddr*>(&hint), sizeof(hint));
	if (connResult == -1)
	{
		std::cout << "Can't connect to server\n" << std::endl;
		close(sock);
		return -1;
	}

	
	enum eBufSize { BUF_SIZE = 4096 };
	char buf[BUF_SIZE];
	std::string userMsg;

	
	do
	{
		std::cout << "> ";
		std::getline(std::cin, userMsg);

		if (userMsg.size() > 0)
		{
			
			int sendResult = send(sock, userMsg.c_str(), userMsg.size() + 1, 0);
			if (sendResult !=-1)
			{
				
				memset(buf,0x00,BUF_SIZE);
				int bytesReceived = recv(sock, buf, BUF_SIZE, 0);
				if (bytesReceived > 0)
				{
					
					std::cout << "SERVER> " << buf << std::endl;
				}
			}
		}

	} while (userMsg.size() > 0);

	close(sock);
	
	return 0;
}
