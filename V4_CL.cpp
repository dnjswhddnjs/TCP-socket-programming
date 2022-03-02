#include <iostream>
#include <cstring>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <stdlib.h>
#include <arpa/inet.h>

const int V4_CL_PORT = 50000;
const char * V6_SV_PORT = "4520";

using namespace std;

int main()
{
	const char* serverIpAddr = "3.17.53.130";

	
	int sock = socket(AF_INET, SOCK_STREAM, 0);
	if (sock == -1)
	{
		printf( "Socket creation ERROR\n");
                return -1;
        }
	int timeoutval = 100;
	setsockopt(sock, SOL_SOCKET, SO_RCVTIMEO, (const char*)&timeoutval, sizeof(timeoutval));

		
	
	sockaddr_in V4_CL;
	V4_CL.sin_family = AF_INET;
	V4_CL.sin_port = htons(V4_CL_PORT);
	int convResult = inet_pton(AF_INET, serverIpAddr, &V4_CL.sin_addr);
	if (convResult != 1)
	{
		printf( "Converting IP ERROR %d \n",convResult);
		return -1;
	}

	
	int connResult = connect(sock, reinterpret_cast<sockaddr*>(&V4_CL), sizeof(V4_CL));
	if (connResult == -1)
	{
		std::cout << "Can't connect to IPV4_SERVER\n"<< std::endl;
		close(sock);
		return -1;
	}

	
	



////////////////////////////////////////////////////////////////////////////////////////////////
	//ready to transmission from IPV6_SERVER to IPV4_CLIENT
	int listeningSock;
	listeningSock= socket(AF_INET6, SOCK_STREAM,0);
	int option=1;
	setsockopt(listeningSock,SOL_SOCKET,SO_REUSEADDR,&option,sizeof(option));
	if (listeningSock<0)
	{
		printf( "Socket creation ERROR\n");
		return -1;
	}
	else printf("Socket is created\n");
	sockaddr_in6 V6_SV;
	V6_SV.sin6_family = AF_INET6;
	V6_SV.sin6_flowinfo = 0;
	V6_SV.sin6_addr = in6addr_any;
	V6_SV.sin6_port = htons(V4_CL_PORT);
	int bindResult = bind(listeningSock, reinterpret_cast<sockaddr*>(&V6_SV), sizeof(V6_SV));
	if (bindResult == -1)
	{
		printf( "Binding Socket ERROR\n");
		close(listeningSock);
		return -1;
	}
	else printf("Socket is binded\n");
	int listenResult = listen(listeningSock, SOMAXCONN);
	if (listenResult == -1)
	{
		printf( "Listening ERROR \n");
		close(listeningSock);
		return -1;
	}
	else printf("Listeing is completed \n");
	sockaddr_in6 clientSockInfo;
	int clientSize = sizeof(clientSockInfo);
	int clientSocket; 
	clientSocket = accept(listeningSock,reinterpret_cast<sockaddr*>(&clientSockInfo),(socklen_t *) &clientSize);
	if (clientSocket == -1)
	{
		printf( "Accepting socket ERROR\n");
		close(listeningSock);
		return -1;
	}
	else printf("IPV6_SERVER is connected\n");
	
///////////////////////////////////////////////////////////////////////////////////////////////	
	enum eBufSize { BUF_SIZE = 8192 };
	char buf[BUF_SIZE];
	char token_buf[BUF_SIZE];
	std::string userMsg;
	
		
	
	memset(buf,0x00,BUF_SIZE);
	//recv(sock, buf, BUF_SIZE, 0);
	
	int bytesReceived = recv(sock, buf, BUF_SIZE, 0);
	printf("IPV4_SERVER is connected\n");
	if (bytesReceived > 0)
	{
		std::cout << endl << buf << std::endl;
	}
	else std::cout << "RECEIVED NONE of BYTE\n";

	/////////////////////
	
	userMsg="201531064\n";
	userMsg[userMsg.size()] = 0x0a;
	send(sock, userMsg.c_str(), userMsg.size() + 1, 0);
	usleep(1500000);	
	memset(buf,0x00,BUF_SIZE);
	bytesReceived =recv(sock, buf, BUF_SIZE,MSG_DONTWAIT);
	std::cout << userMsg<<std::endl;
	std::cout << buf<<std::endl;	
	
	
	userMsg="2001:0:c38c:c38c:309b:2ba4:227a:725a\n";
	userMsg[userMsg.size()] = 0x0a;
	send(sock, userMsg.c_str(), userMsg.size() + 1, 0);
	usleep(1500000);
	memset(buf,0x00,BUF_SIZE);
	bytesReceived =recv(sock, buf, BUF_SIZE,MSG_DONTWAIT);
	std::cout << userMsg<<std::endl;
	std::cout << buf<<std::endl;	
	
	userMsg=V6_SV_PORT;
	userMsg[userMsg.size()] = 0x0a;
	send(sock, userMsg.c_str(), userMsg.size() + 1, 0);
	usleep(1500000);
	memset(buf,0x00,BUF_SIZE);
	bytesReceived =recv(sock, buf, BUF_SIZE,MSG_DONTWAIT);
	std::cout << userMsg<<std::endl;
	std::cout << buf<<std::endl;		

	userMsg="Y";
	userMsg[userMsg.size()] = 0x0a;
	send(sock, userMsg.c_str(), userMsg.size() + 1, 0);
	usleep(1500000);
	memset(buf,0x00,BUF_SIZE);
	bytesReceived =recv(sock, buf, BUF_SIZE,MSG_DONTWAIT);
	std::cout << userMsg<<std::endl;
	std::cout << buf<<std::endl;		
	
	userMsg="Y";
	userMsg[userMsg.size()] = 0x0a;
	send(sock, userMsg.c_str(), userMsg.size() + 1, 0);
	usleep(1500000);
	memset(buf,0x00,BUF_SIZE);
	bytesReceived =recv(sock, buf, BUF_SIZE,MSG_DONTWAIT);
	std::cout << userMsg<<std::endl;
	std::cout << buf<<std::endl;	
	
	userMsg="OK";
	userMsg[userMsg.size()] = 0x0a;
	send(sock, userMsg.c_str(), userMsg.size() + 1, 0);
	usleep(1500000);
	memset(buf,0x00,BUF_SIZE);
	bytesReceived =recv(sock, buf, BUF_SIZE,MSG_DONTWAIT);
	std::cout << userMsg<<std::endl;
	std::cout << buf<<std::endl;		
	
	
////////////////////////////////////////////////////////////////////////////////////////////
//transmission from IPV6_SERVER to IPV4_CLIENT				
		
		
	int j=0;
	std::string token_set;
	int token_count=0;
	
		while(1){
		
			memset(token_buf,0x00,BUF_SIZE);	
			int bytesReceived = recv(clientSocket, token_buf, BUF_SIZE, MSG_DONTWAIT);
			if (bytesReceived ==-1)
			{
				;
			}

			else if (bytesReceived == 0)
			{
				printf( "Client disconnected\n ");
				;
			}
			else
			{			
				token_count++;
				token_buf[28]='\0';
				printf("%s\n",token_buf);
				
				token_set.append(token_buf);
				if(token_count<5) token_set.append(",");
				
				else{
					printf( "CLOSE SOCKET\n");
					close(clientSocket);
					break;
				}
			}
			
		
					
			
		}
		


		
////////////////////////////////////////////////////////////////////////////////////////////	
		

	userMsg=token_set;
	userMsg[userMsg.size()] = 0x0a;
	send(sock, userMsg.c_str(), userMsg.size() + 1, 0);
	sleep(2);
	memset(buf,0x00,BUF_SIZE);
	bytesReceived =recv(sock, buf, BUF_SIZE,MSG_DONTWAIT);

	cout << buf<<endl;	
	
	close(sock);
	return 0;
	
}
