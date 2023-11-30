#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>

int main() {
	//variable initialization
	char welcome[]= "Welcome to ENSEA shell.\nType 'exit' to quit.\n";
	char prompt[]= "enseash %";
	
	write(STDOUT_FILENO, welcome, strlen(welcome)); //STDOUT_FILENO represents the standard output 
	write(STDOUT_FILENO, prompt, strlen(prompt));
	return 0;
}
