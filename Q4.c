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
	char exitMsg[]= "Bye Bye...\n";
	char status_exit[100];
	char status_sign[100];
	
	write(STDOUT_FILENO, welcome, strlen(welcome)); //STDOUT_FILENO represents the standard output 
	write(STDOUT_FILENO, prompt, strlen(prompt));	


	while(1) {	
		
		ssize_t length = read(STDIN_FILENO, command, 1023);
		command[length] = '\0';                         //character string cleaning
        if (command[length - 1] == '\n') {
            command[length - 1] = '\0';
		}
		
		if (length <= 0) {
			if (length == 0) { // EOF (Ctrl+D)
				write(STDOUT_FILENO, "\n", strlen("\n"));
			}
			break;
		}

 
		if (strcmp(command, "exit") == 0 || length == 0) {                //execute if the command is "exit"
			write(STDOUT_FILENO, exitMsg, strlen(exitMsg));	
            break;
						}
		
		pid_t pid = fork();             //child process to run command
		if (pid ==0){
				
				execlp(command, command, (char *)NULL);
				break;
				}
		else{
			//parent process
			int status;
			waitpid(pid, &status, 0);
			if (WIFEXITED(status)){
				snprintf(status_exit, 100, "enseash [exit:%d] %% ", WEXITSTATUS(status));
				write(STDOUT_FILENO, status_exit, strlen(status_exit));
				}
			else if (WIFSIGNALED(status)){
				snprintf(status_sign, 100, "enseash [sign:%d] %% ", WTERMSIG(status));
                write(STDOUT_FILENO, status_sign, strlen(status_sign));
				}
			
				}
		}
	return 0;
}
