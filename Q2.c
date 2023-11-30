#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>

int main() {
	//variable initialization
	char command[1024];
	char welcome[]= "Welcome to ENSEA shell.\nType 'exit' to quit.\n";
	char prompt[]= "enseash % ";
	
	write(STDOUT_FILENO, welcome, strlen(welcome)); //STDOUT_FILENO represents the standard output 



	while(1) {
		write(STDOUT_FILENO, prompt, strlen(prompt));	

		ssize_t length = read(STDIN_FILENO, command, 1023);
		command[length] = '\0';                         //character string cleaning
        if (command[length - 1] == '\n') {
            command[length - 1] = '\0';
		}

		pid_t pid = fork();               //child process to run command
			if (pid ==0){
				if (strcmp(command, "date") == 0) {                //execute if the command is "date"
                			execlp("date", "date", (char *)NULL);
                			
            			} 
            	else if (strcmp(command, "fortune") == 0) {                //execute if the command is "fortune"
                			execlp("fortune", "fotune", (char *)NULL);
						}

				else {                  //the command is not recognized
					write(STDERR_FILENO, "Unknown command\n", strlen("Unknown command\n"));
					
					}
				}
			else{
				int status;              //parent process
				waitpid(pid, &status, 0);
				}
		}
	return 0;
}
