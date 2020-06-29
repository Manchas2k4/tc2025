#include "header.h"

int main(int argc, char* argv[]) {
    int sfd, number, answer, continuE;
	int port, guess;
	struct sockaddr_in server_info;
	
	if (argc != 3) {
	    printf("usage: %s ip port\n", argv[0]);
	    return -1;
	}
	
	port = atoi(argv[2]);
	if (port < 5000) {
		printf("%s: The port must be greater than 5000.\n", argv[0]);
		return -1;
	}
	
	if ( (sfd = socket(AF_INET, SOCK_STREAM, 0)) < 0 ) {
		perror(argv[0]);
		return -1;
	}
	
	server_info.sin_family = AF_INET;
	server_info.sin_addr.s_addr = inet_addr(argv[1]);
	server_info.sin_port = htons(port);
	if ( connect(sfd, (struct sockaddr *) &server_info, sizeof(server_info)) < 0 ) {
		perror(argv[0]);
		return -1;
	}
	
	do {
		guess = 0;
		while (!guess) {
			printf("Give me the number: ");
			scanf("%i", &number);
			
			write(sfd, &number, sizeof(number));
			read(sfd, &answer, sizeof(answer));
			
			switch (answer) {
				case SMALLER: printf("The number is smaller.\n"); break;
				case BIGGER : printf("The number is bigger.\n"); break;
				default     : guess = 1; break;
			}
		}
		printf("You guessed it! Congratulations!\n");
		printf("Continue? (0/1)? ");
		scanf("%i", &continuE);
		
		answer = (continuE == 1)? CONTINUE : END;
		write(sfd, &answer, sizeof(answer));
	} while (continuE == 1);
	close(sfd);
	return 0;
}
