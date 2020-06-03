#include <stdio.h>
#include <stdlib.h>

int main(int argc, char ** argv) {
	// fflush is better
	setlinebuf(stdout);
	char lijn[BUFSIZ];
	while( fgets(lijn, BUFSIZ, stdin) != NULL) {
		int a,b;
		if(sscanf(lijn, "%d %d", &a, &b) != 2) {
			fprintf(stderr, "Failed to read two values from stdin!\n");
			exit(1);
		}
		printf("%d\n", a+b);
		// force flush to buffer so this program works with pipes
		fflush(stdout);
	}
	return 0;
}
