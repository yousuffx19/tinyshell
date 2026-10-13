#include <string.h>
#include <stdlib.h>
#include <stdio.h>

int get_tokens(char *command, char *args[]){
	char token[100];
	int ntokens = 0;
	int tokenp = 0;
	for(int i = 0; i < (int) strlen(command); i++){
		if(command[i] == ' '){
			token[tokenp] = '\0';
			tokenp = 0;
			args[ntokens] = malloc(100);
			strcpy(args[ntokens], token);
			ntokens++;
			continue;
		}
		token[tokenp] = command[i];
		tokenp++;
	}
	//copy last word into args
	token[tokenp] = '\0';
	args[ntokens] = malloc(100);
	strcpy(args[ntokens], token);
	ntokens++;
	args[ntokens] = NULL;
	return ntokens;
}

int setup_pipes(char *command, char *sub_commands[]){
	char sub_command[512];
	int sp = 0, cp = 0;
	for(int i = 0; i < (int) strlen(command);i++){
		if(command[i] == '|'){
			sub_command[cp] = '\0';
			sub_commands[sp] = malloc(100);
			strcpy(sub_commands[sp], sub_command);
			sp++;
			cp = 0;
			continue;
		}
		sub_command[cp] = command[i];
		cp++;
	}
	sub_command[cp] = '\0';
	sub_commands[sp] = malloc(100);
	strcpy(sub_commands[sp], sub_command);
	sp++;
	return sp;	
}


