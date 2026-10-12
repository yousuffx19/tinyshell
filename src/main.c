#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/wait.h>
#include "parser.h"
#include "utils.h"

char command_buffer[100]; 
char CWD[100];

void exec_command(char *command){
	char *args[20];
	get_tokens(command, args);
        execvp(args[0], args);
	printf("Invalid Command\n");
	exit(1);
}

int main(){
	printf("\033[H\033[2J");
	getcwd(CWD, 100);
	while(1){
	printf("\e[0;35mtinyshell:\033[0m%s > ", CWD);
	fgets(command_buffer, 100, stdin);
	command_buffer[strlen(command_buffer)-1] = '\0';
	int is_cd = set_working_directory(command_buffer);
	if(is_cd == -1){
		printf("Invalid path\n");
		continue;
	}
	else if(is_cd == 1){
		getcwd(CWD, 100);
		continue;
	}
	if(!strcmp(command_buffer, "exit")){
		exit(0);
	}
	char *commands[20];
	int num_pipes = setup_pipes(command_buffer, commands);
	int fds[2];
	int prev_read = -1;
	for(int i = 0; i < num_pipes; i++){
		if(i < num_pipes-1){
			pipe(fds);
		}
		pid_t p = fork();
		if(p == 0){
			if(prev_read != -1){
				dup2(prev_read, STDIN_FILENO);
				close(prev_read);
			}
			if(i < num_pipes-1){
				dup2(fds[1], STDOUT_FILENO);
				close(fds[1]);
			}
			exec_command(commands[i]);
		}
		else if(p < 0){
			printf("Error in creating a child process\n");
			exit(1);
		}
		if(prev_read != -1){
			close(prev_read);
		}
		if(i < num_pipes-1){
			close(fds[1]);
			prev_read = fds[0];
		}
	}
	int status = 0;
	while(wait(&status) > 0);
	}
	return 0;
}
