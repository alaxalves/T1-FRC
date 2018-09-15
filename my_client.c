#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <unistd.h>
#include <fcntl.h>

#define BUFFER 512
#define PORT 8080

void kill(char *error){
	perror(error);
	exit(1);
}

int main(int argc, char const *argv[]){
	struct sockaddr_in server_address;
	bzero(&server_address, sizeof(server_address));

	int my_socket;
	my_socket = socket(AF_INET, SOCK_DGRAM, 0);
	server_address.sin_family = AF_INET;
	server_address.sin_addr.s_addr = INADDR_ANY; //Change to server custom IP
	server_address.sin_port = htons(PORT);

	char *message = "Hello ";

	int fd, buffer_length;
	char buffer[BUFFER];

	fd = open("/home/lucas/lappis/portal/noosfero/app/controllers/my_profile/cms_controller.rb", O_RDONLY);
	while((buffer_length = read(fd, buffer, BUFFER)) > 0){
		buffer[buffer_length] = 0;
		printf("%s\n", buffer);
		sendto(my_socket, buffer, buffer_length, MSG_CONFIRM,
			(const struct sockaddr *) &server_address, sizeof(server_address));
	}

	char *end_message = "FILE_END";
	sendto(my_socket, end_message, strlen(end_message), MSG_CONFIRM,
			(const struct sockaddr *) &server_address, sizeof(server_address));

	return 0;
}