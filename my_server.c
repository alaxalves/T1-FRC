#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <unistd.h>
#include <fcntl.h>
#include <arpa/inet.h>
#include <sys/socket.h>

#define BUFFER 512
#define PORT 8080

void kill(char *error){
	perror(error);
	exit(1);
}

int main(int argc, char const *argv[]){
	struct sockaddr_in server_address, client_address;
	bzero(&server_address, sizeof(server_address));
	bzero(&client_address, sizeof(client_address));

	int my_socket;
	my_socket = socket(AF_INET, SOCK_DGRAM, 0);
	server_address.sin_family = AF_INET;
	server_address.sin_addr.s_addr = INADDR_ANY; //Change to client custom IP
	server_address.sin_port = htons(PORT);

	if(bind(my_socket, (const struct sockaddr *) &server_address, 
		sizeof(server_address)) < 0){
		kill("bind");
	}

	int fd, buffer_length = BUFFER;
	char buffer[BUFFER];
	int received_length = sizeof(client_address);

	fd = open("cmscontroller.rb", O_RDWR | O_CREAT, 0666);

	printf("Esperando...\n");  
  while((buffer_length = recvfrom(my_socket, buffer, buffer_length, MSG_WAITALL, (struct sockaddr *) &client_address, &received_length))){
		buffer[buffer_length] = 0;
		if(!strcmp(buffer, "FILE_END")){
			break;
		}
		printf("%s", buffer);
		write(fd, buffer, buffer_length);
  }
  close(fd);

	return 0;
}