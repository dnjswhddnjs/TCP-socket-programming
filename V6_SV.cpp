#include <iostream>
#include <cstring>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <stdlib.h>
#include <arpa/inet.h>


const int V6_SV_PORT =4520;
const int V4_CL_PORT=50000;



using namespace std;


int main() {

	

	const int NI_MAXHOST=1025;
	const int NI_MAXSERV=32;

	int listeningSock;
	enum eBufSize { BUF_SIZE = 8192 };
	char buf[BUF_SIZE];


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
	V6_SV.sin6_port = htons(V6_SV_PORT);
	

	
	
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
	
	
	
////////////////////////////////////////////////////////////////////////////////////////////////
	//ready to transmission from IPV6_SERVER to IPV4_CLIENT	
	const char* serverIpAddr = "0::1";
	int sock = socket(AF_INET6, SOCK_STREAM, 0);
	if (sock == -1)
	{
		printf( "Socket creation ERROR\n");
		return -1;
	}
	
	sockaddr_in6 V4_CL;
	V4_CL.sin6_family = AF_INET6;
	V4_CL.sin6_flowinfo = 0;
	V4_CL.sin6_port = htons(V4_CL_PORT);

	int convResult = inet_pton(AF_INET6, serverIpAddr, &V4_CL.sin6_addr);
	if (convResult != 1)
	{
		printf( "Converting IP ERROR %d \n",convResult);
		
		return -1;
	}
	else printf("Converting IP is completed\n");
	while (connect(sock, reinterpret_cast<sockaddr*>(&V4_CL), sizeof(V4_CL)) == -1)
	{
		printf( "Trying to connect to V4_CL\n");
		sleep(2);
	}
	printf("Connected to V4_CL\n");
	
	
	
	
////////////////////////////////////////////////////////////////////////////////////////////////
		
	

	sockaddr_in6 clientSockInfo;
	int clientSize = sizeof(clientSockInfo);
	int clientSocket; 
	pid_t pid;
	int parent_pid=0;
	parent_pid=getpid();


	
	int i=1;
	
	
	for(i =1;i<=5;i++) pid=fork();
			
	
	
	
	int waiting_count=0;
	
	for( ; ; ){
		
		if(parent_pid==getpid()){

			sleep(2);
			waiting_count++;
			printf("Waiting...\n");
			if(waiting_count>=12)
			{ 			
				break;
			}
			 
			else continue;
		}

		
		//printf("ready to accept\n\n");
		

		
		clientSocket = accept(listeningSock, reinterpret_cast<sockaddr*>(&clientSockInfo),(socklen_t *) &clientSize);
		if (clientSocket == -1)
		{
			printf( "Accepting socket ERROR \n");
			close(listeningSock);
			close(sock);
			return -1;
		}
		else printf("Client socket is accepted\n");
		
		
		
		
		if(getpid()!=parent_pid){
		
		
		
		
			close(listeningSock);
			
	
		while (true)
		{

		
			memset(buf,0x00,BUF_SIZE);
		
			
			
			int bytesReceived = recv(clientSocket, buf, BUF_SIZE, 0);
			if (bytesReceived ==-1)
			{
				printf( " Quitting\n");
				break;
			}

			else if (bytesReceived == 0)
			{
				printf( "Client disconnected\n ");
				break;
			}
			else
			{			

				printf("%s",buf);
				sleep(2);
				send(sock, buf, bytesReceived + 1, 0);
				printf("Token was sent to V4_CL\n");
				

				
				close(clientSocket);
				return 0;
			}

		}
		
		}
		

	
	}


	printf("Server is closed\n");
	close(listeningSock);
	
	
	return 0;
	

}
