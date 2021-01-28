#include <stdio.h> 
#include <sys/socket.h> 
#include <arpa/inet.h> 
#include <unistd.h> 
#include <string.h> 

#include <iostream>

constexpr int port = 8080;

int main(int argc, char const *argv[]) 
{ 
	int sock = 0; 
    int valread = 0;
	struct sockaddr_in serv_addr; 
	const char *order = argv[1]; 
	char buffer[1024] = {0}; 

	if ((sock = socket(AF_INET, SOCK_STREAM, 0)) < 0) 
	{ 
		printf("\n Socket creation error \n"); 
		return 1; 
	} 

	serv_addr.sin_family = AF_INET; 
	serv_addr.sin_port = htons(port); 
	
	if (inet_pton(AF_INET, "127.0.0.1", &serv_addr.sin_addr) <= 0)
	{ 
		printf("\nInvalid address/ Address not supported \n"); 
		return 1; 
	} 

	if (connect(sock, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0) 
	{ 
		printf("\nConnection Failed \n"); 
		return 1; 
	} 

	send(sock, order , strlen(order) , 0 ); 

	printf("Order message sent\n"); 

    while (true)
    {
        valread = read(sock, buffer, 1024); 
        if (valread == 0)
        {
            std::cout << "Server is not reachable." << std::endl;
            exit(0);
        }
        printf("%s\n", buffer);
    }
	return 0; 
} 

