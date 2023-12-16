#include "../include/client.h"




int main(int argc  , char **argv){
   

   int PORT = atoi(argv[1]);
    int clientSocket;
	struct sockaddr_in serverAddr;
	char buffer[1024];

	clientSocket = socket(PF_INET, SOCK_STREAM, 0);
	printf("[+]Client Socket Created Sucessfully.\n");

	memset(&serverAddr, '\0', sizeof(serverAddr));
	serverAddr.sin_family = AF_INET;
	serverAddr.sin_port = htons(PORT);
	serverAddr.sin_addr.s_addr = inet_addr("127.0.0.1");

	connect(clientSocket, (struct sockaddr*)&serverAddr, sizeof(serverAddr));
	printf("[+]Connected to Server.\n");

	recv(clientSocket, buffer, 1024, 0);
	printf("[+]Data Recv: %s\n", buffer);
// wait for user input

    char input[1024];

    while(1){
        printf("Enter a message: ");
        fgets(input, 1024, stdin);
        send(clientSocket, input, strlen(input), 0);
        printf("Message sent\n");

        // or recv(clientSocket, buffer, 1024, 0);
        // printf("Message received: %s\n", buffer);

     if (strcmp(input, "exit\n") == 0){
            printf("Client exiting\n");
            break;
        }

        // check if server sent message
        if (recv(clientSocket, buffer, 1024, 0) > 0){
            printf("Message received: %s\n", buffer);
        }

        
    }







    return 0;
}