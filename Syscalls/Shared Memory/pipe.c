#include <unistd.h>
#include <sys/wait.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdio.h>
#include <string.h>

int main(int argc, char **argv) {
	int fd_PC[2];
	int fd_CP[2];
	char buffer[BUFSIZ];

	if(pipe(fd_PC) < 0) {
		perror(argv[0]);
		return 1;
	}

	if(pipe(fd_CP) < 0) {
		perror(argv[0]);
		return 1;
	}
	int pid;

	if( (pid = fork()) < 0) {
		perror(argv[0]);
		return 1;
	}
	else if( pid == 0) {
		// child process
		// close child->parent read fd 
		close(fd_CP[0]);
		// close parent->child write fd
		close(fd_PC[1]);
		read(fd_PC[0], buffer, BUFSIZ);
		int a, b;
		sscanf(buffer, "%d %d", &a, &b);
		// we have to reset the buffer or when the new buffers length is smaller, the remaining chars of the previous buffer will be printed too
		memset(buffer, 0, sizeof(buffer));
		sprintf(buffer, "%d\n", a+b);
		write(fd_CP[1], buffer, strlen(buffer));
		// close the other fd's
		close(fd_CP[1]);
		close(fd_PC[0]);
		return 0;
	}
	// parent proces
	// close parent->child read fd 
	close(fd_PC[0]);
	// close child->parent write fd
	close(fd_CP[1]);
	// read the input from stdin
	fgets(buffer, BUFSIZ, stdin);
	// send that line to child
	write(fd_PC[1], buffer, strlen(buffer));
	// we have to reset the buffer or when the new buffers length is smaller, the remaining chars of the previous buffer will be printed too
	memset(buffer, 0, sizeof(buffer));
	read(fd_CP[0], buffer, BUFSIZ);
	printf("%s", buffer);
	waitpid(pid, NULL, 0);
	return 0;
}
