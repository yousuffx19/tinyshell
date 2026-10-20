#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>
#include <sys/wait.h>
#include "parser.h"
#include "utils.h"
#include "terminal.h"

char command_buffer[1024]; 
char CWD[100];

int setup_redirections(char *tokens[], int num_tokens, char *command_args[]){
        int num_redirs = 0;
	int fd;
        for(int i = 0; i < num_tokens; i++)
        {	
		if(i < num_tokens - 1){
		if(!strcmp(tokens[i], ">")){
			if((fd = open(tokens[i+1], O_WRONLY|O_CREAT|O_TRUNC, 0644)) < 0){
				printf("Error in opening file: %s\n", tokens[i+1]);
				exit(1);
			}
			dup2(fd, STDOUT_FILENO);
			close(fd);
                        num_redirs++;
                }
		if(!strcmp(tokens[i], ">>")){
                        if((fd = open(tokens[i+1], O_WRONLY|O_CREAT|O_APPEND, 0644)) < 0){
                                printf("Error in opening file: %s\n", tokens[i+1]);
                                exit(1);
                        }
			dup2(fd, STDOUT_FILENO);
			close(fd);
                        num_redirs++;
                }
		if(!strcmp(tokens[i], "2>")){
                        if((fd = open(tokens[i+1], O_WRONLY|O_CREAT|O_TRUNC, 0644)) < 0){
                                printf("Error in opening file: %s\n", tokens[i+1]);
                                exit(1);
                        }
                        dup2(fd, STDERR_FILENO);
			close(fd);
                        num_redirs++;
                }
		if(!strcmp(tokens[i], "2>>")){
                        if((fd = open(tokens[i+1], O_WRONLY|O_CREAT|O_APPEND, 0644)) < 0){
                                printf("Error in opening file: %s\n", tokens[i+1]);
                                exit(1);
                        }
                        dup2(fd, STDERR_FILENO);
			close(fd);
                        num_redirs++;
                }
		if(!strcmp(tokens[i], "&>") && !strcmp(tokens[i], ">&")){
                        if((fd = open(tokens[i+1], O_WRONLY|O_CREAT|O_TRUNC, 0644)) < 0){
                                printf("Error in opening file: %s\n", tokens[i+1]);
                                exit(1);
                        }
                        dup2(fd, STDOUT_FILENO);
			dup2(fd, STDERR_FILENO);
                        num_redirs++;
                }
		if(!strcmp(tokens[i], "<")){
                        if((fd = open(tokens[i+1], O_RDONLY, 0644)) < 0){
                                printf("Error in opening file: %s\n", tokens[i+1]);
                                exit(1);
                        }
                        dup2(fd, STDIN_FILENO);
                        num_redirs++;
                }
		}
		if(num_redirs == 0){
			command_args[i] = malloc(32);
			strcpy(command_args[i], tokens[i]);
			command_args[i+1] = NULL;
		}
        }
        return num_redirs;
}

void exec_command(char *command){
	int spaces = strip_start_space(command);
	command += spaces;
	strip_end_space(command);
	char *tokens[30];
	char *args[20];
	int num_tokens = get_tokens(command, tokens);
	setup_redirections(tokens, num_tokens, args);
        execvp(args[0], args);
	printf("Invalid Command\n");
	exit(1);
}

int main(){
	printf("\033[H\033[2J");
	getcwd(CWD, 100);
	while(1){
	printf("\e[0;35mtinyshell:\033[0m%s > ", CWD);
	fflush(stdout);
	read_input(command_buffer);
	command_buffer[strlen(command_buffer)] = '\0';
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
