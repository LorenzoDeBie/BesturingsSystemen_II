#include <unistd.h>
#include <sys/wait.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

int main(int argc, char **argv) {
	int fd_PC[2];
	int fd_CP[2];
	char buffer[BUFSIZ];

	if(pipe(fd_PC) < 0) {
		perror(argv[0]);
		return 1;
	}

	if(pipe(fd_CP) < 0) {
		close(fd_PC[0]);
		close(fd_PC[1]);
		perror(argv[0]);
		return 1;
	}
	int pid;

	if( (pid = fork()) < 0) {
		close(fd_CP[0]);
		close(fd_CP[1]);
		close(fd_PC[0]);
		close(fd_PC[1]);
		perror(argv[0]);
		return 1;
	}
	else if( pid == 0) {
		// child process
		// close child->parent read fd 
		close(fd_CP[0]);
		// close parent->child write fd
		close(fd_PC[1]);
		// duplicate stdin of child to pipe
		if( dup2(fd_PC[0], 0) < 0) {
			close(fd_CP[0]);
			close(fd_CP[1]);
			close(fd_PC[0]);
			close(fd_PC[1]);
			perror(argv[0]);
			return 1;
		}
		// duplicate stdout of child to pipe
		if( dup2(fd_CP[1], 1) < 0 ) {
			close(fd_CP[0]);
			close(fd_CP[1]);
			close(fd_PC[0]);
			close(fd_PC[1]);
			perror(argv[0]);
			return 1;
		}
		if ( execlp("./bin/child", "child", (char *) NULL) < 0) {
			perror(argv[0]);
			exit(1);
		}
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
	// close the fd since we don't need it anymore
	close(fd_PC[1]);
	// we have to reset the buffer or when the new buffers length is smaller, the remaining chars of the previous buffer will be printed too
	memset(buffer, 0, sizeof(buffer));
	read(fd_CP[0], buffer, BUFSIZ);
	// close the fd since we don't need it anymore
	close(fd_CP[0]);
	printf("%s", buffer);
	waitpid(pid, NULL, 0);
	return 0;
}
