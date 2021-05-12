#include "header.h"
#include <time.h>

void put_msg() {
  Message msg;
  int i, msqid, num_msg, length;
  long msg_type;
  key_t key;

  if ( (key = ftok("/dev/null", 24)) == (key_t) -1 ) {
    perror("ftok");
    exit(-1);
  }

  if ( (msqid = msgget(key, 0666)) == -1 ) {
    perror("msgget");
    exit(-1);
  }

  srand(getpid());
  length = sizeof(Message) - sizeof(long);
  num_msg = rand() % 6 + 1;
  for (i = 0; i < num_msg; i++) {
    msg.type = rand() % 4 + 1;
    msg.process = getpid();
    msg.value = rand() % 100 + 1;
    msgsnd(msqid, &msg, length, 0);
    printf("MESSAGE POSTED: msg_type = %li process = %i value = %i\n", msg.type, msg.process, msg.value);
    sleep(1);
  }
}

int main(int argc, char* argv[]) {
  int amount = 0, semid, i, pid;
	key_t key;

	if (argc != 2) {
		printf("usage: %s amount\n", argv[0]);
		return -1;
	}

	amount = atoi(argv[1]);
	if (amount <= 0) {
		printf("%s: The amount must be a positive number\n", argv[0]);
		return -1;
	}

	for (i = 0; i < amount; i++) {
		if ( (pid = fork()) < 0 ) {
			perror("fork");
			return -1;
		} else if (pid == 0) {
			put_msg();
		} else {
		}
	}

	while (i > 0) {
		wait(NULL);
		i--;
	}
	return 0;
}
