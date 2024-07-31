#include <stdio.h>
#include <unistd.h>

int main(void) {
	pid_t pid = fork();

	if(pid > 0) {
		printf("Parent process\n");
	}
	else if(pid == 0) {
		pid_t pid_2 = fork();
		if(pid_2 > 0) {
			printf("Second child\n");
		}
		else if(pid_2 == 0) {
			printf("Child process\n");
		}
		else {
			fprintf(stderr, "Second fork failed\n");
		}
	}
	else {
		fprintf(stderr, "Fork failed\n");
	}
	return 0;
}
