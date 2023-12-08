#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>
#include <time.h>
#include <fcntl.h>

int main() {
	//variable initialization
	char command[1024];
	char welcome[]= "Welcome to ENSEA shell.\nType 'exit' to quit.\n";
	char prompt[]= "enseash % ";
	char exitMsg[]= "Bye Bye...\n";
	char status_exit[100];
	char status_sign[100];
	struct timespec start_time,end_time;
	write(STDOUT_FILENO, welcome, strlen(welcome)); //STDOUT_FILENO represents the standard output 
	write(STDOUT_FILENO, prompt, strlen(prompt));	
    long execution_time=0;
    int status;

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
		//timer start
		clock_gettime(CLOCK_MONOTONIC, &start_time);
		
		if (pid ==0){
		    char *input_file = NULL;
			char *output_file = NULL;
			char *token = strtok(command, " ");
			char *argv[1024];
			int argc = 0;
        
			while (token != NULL) {
				if (strcmp(token, "<") == 0) {
					token = strtok(NULL, " ");
					input_file = token;
				} else if (strcmp(token, ">") == 0) {
					token = strtok(NULL, " ");
					output_file = token;
				} else {
					argv[argc++] = token;
				}	
				token = strtok(NULL, " ");
				}
			argv[argc] = NULL;

			//redirectin STDIN
			if (input_file != NULL) {
				int fd_in = open(input_file, O_RDONLY);
				dup2(fd_in, STDIN_FILENO);
				close(fd_in);
			}
			//redirecting STDOUT
			if (output_file != NULL) {
				int fd_out = open(output_file, O_WRONLY | O_CREAT | O_TRUNC, 0644);
				dup2(fd_out, STDOUT_FILENO);
				close(fd_out);
			}	

			//order processing
			execvp(argv[0], argv);
				
				
			break;
				}
		else{
			//parent process
			
			
			wait(&status);
			//timer stop
			clock_gettime(CLOCK_MONOTONIC, &end_time);
			//conversion 
			execution_time = (end_time.tv_nsec - start_time.tv_nsec)/1000000 + (end_time.tv_sec - start_time.tv_sec)*1000;
		
			
			if (WIFEXITED(status)){
				
				snprintf(status_exit, 100, "enseash [exit:%d|%ld ms] %% ", WEXITSTATUS(status), execution_time);
				write(STDOUT_FILENO, status_exit, strlen(status_exit));
				}
				
			else if (WIFSIGNALED(status)){
				
				snprintf(status_sign, 100, "enseash [sign:%d|%ld ms] %% ", WTERMSIG(status), execution_time);
                write(STDOUT_FILENO, status_sign, strlen(status_sign));
				}
			
				}
		}
	return 0;
}
