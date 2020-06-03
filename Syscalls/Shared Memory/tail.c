#include <sys/stat.h>
#include <fcntl.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>

int main(int argc, char **argv) {
	// parse options
	int opt, nlines = 10;
	while( (opt = getopt(argc, argv, "n:")) != -1) {
		switch(opt) {
			case 'n':
				nlines = atoi(optarg);
				break;
			default:
				fprintf(stderr, "Usage: %s [-n nLines] file\n", argv[0]);
				exit(1);
		}

	}
	// check args
	if(optind >= argc) {
		fprintf(stderr, "Need a file!\n");
		exit(1);
	}
	// open file
	int fd;
	if( (fd = open(argv[optind], O_RDONLY)) < 0) {
		perror(argv[0]);
		exit(1);
	}
	// array to hold the lines
	char **lines = malloc(sizeof(char*) * nlines);
	int currentLine = nlines - 1, currentLength = 0;
	// go to the last char of the file
	lseek(fd, -1, SEEK_END);
	// var to hold current char
	char current;
	read(fd, &current, 1);
	// we don't want to read the last empty line
	if(current == '\n') 
		lseek(fd, -2, SEEK_END);
	else 
		lseek(fd, -1, SEEK_END);

	// loop over all chars in reverse order
	while ( lseek(fd, 0, SEEK_CUR) > 0 && currentLine >= 0) {
		// read 1 char
		read(fd, &current, 1);
		// we got a line!
		if(current == '\n') {
			// not an empty line
			if(currentLength != 0) { 
				// malloc the size needed
				lines[currentLine] = malloc(sizeof(char) * currentLength);
				// read the whole line
				int n = read(fd, lines[currentLine], currentLength);
				// go back to before the line
				lseek(fd, -currentLength, SEEK_CUR);
				// reset the length
				currentLength = 0;
			}
			// empty line
			else {
				// just set the pointer to NULL
				lines[currentLine] = NULL;
			}
			// decrement line index
			currentLine--;
		}
		// not a new line
		else {
			// increment the length counter
			currentLength++;
		}
		// go back 2 chars
		lseek(fd, -2, SEEK_CUR);
	}
	if( lseek(fd, 0, SEEK_CUR) == 0) {
		lines[currentLine] = malloc(sizeof(char) * currentLength + 1);
		read(fd, lines[currentLine--], currentLength + 1);
	}
	
	//print out the last ten lines
	for(currentLine++; currentLine < nlines; currentLine++) {
		if(lines[currentLine]) {
			write(1, lines[currentLine], strlen(lines[currentLine]));
			free(lines[currentLine]);
		}
		write(1, "\n", 1);
	}
	free(lines);
	return 0;
}
