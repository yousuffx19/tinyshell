#include <stdio.h>
#include <stdlib.h>
#include <termios.h>
#include <unistd.h>
#include <ctype.h>
#include <string.h>

struct termios original;
char input_buffer[1024];
char history_buffer[1024][1024];
int cursor;
int history_cursor = 0;
int history_length = 0;

void save_to_history(char *command){
	if(history_length > 1024){
		history_length = 0;
	}
	if(history_length > 0 && !strcmp(history_buffer[history_length-1], command)){
		return;
	}
	strcpy(history_buffer[history_length], command);
	history_length++;
	history_cursor = history_length;
}

void get_last_command(char *buffer){
	if(history_cursor > 0){
		strcpy(buffer, history_buffer[history_cursor-1]);
		history_cursor--;
	}
}

void get_next_command(char *buffer){
	if(history_cursor < history_length){
		strcpy(buffer, history_buffer[history_cursor+1]);
		history_cursor++;
	}
	if(history_cursor == history_length){
		strcpy(buffer, "\0");
	}
}

void disable_raw_mode(){
	tcsetattr(STDIN_FILENO, TCSAFLUSH, &original);
}

void handle_escape(){
	char seq[2];
	read(STDIN_FILENO, seq, 2);
	if(!strcmp(seq, "[D\0")){
		if(cursor > 0){
		cursor--;
		}
	}
	if(!strcmp(seq, "[C\0")){
		if(cursor < (int) strlen(input_buffer)){
		cursor++;}
	}
	if(!strcmp(seq, "[A\0")){
		get_last_command(input_buffer);
		cursor = strlen(input_buffer);
	}
	if(!strcmp(seq, "[B\0")){
		get_next_command(input_buffer);
		cursor = strlen(input_buffer);
	}
}

void handle_cntrl(char a){
	if(a == 127){
		int len = (int) strlen(input_buffer);
		if(cursor == 0){
			return;
		}
		for(int i = cursor-1; i < len; i++){
			input_buffer[i] = input_buffer[i+1];
		}
		cursor--;
	}
	if(a == 10){
		return;
	}
	if(a == 27){
		handle_escape();
		return;
	}
}

void read_input(char *command_buffer){
	struct termios raw;
	cursor = 0;
	tcgetattr(STDIN_FILENO, &original);
	raw = original;
	raw.c_lflag &= ~(ECHO | ICANON);
	tcsetattr(STDIN_FILENO, TCSAFLUSH, &raw);
	atexit(disable_raw_mode);
	input_buffer[0] = '\0';
	char a;
	while(read(STDIN_FILENO, &a, 1) == 1 && a != 10){
		if(cursor > 0){
                printf("\033[%dD", cursor);}
                printf("\x1b[K"); //clear line
		fflush(stdout);
		if(!iscntrl(a)){
			int len = (int) strlen(input_buffer);
			for(int i = len; i > cursor; i--){
				input_buffer[i] = input_buffer[i-1];
			}
			input_buffer[cursor] = a;
			cursor++;
			input_buffer[len+1] = '\0';
		}
		else{
		handle_cntrl(a);
		}
		printf("%s", input_buffer);
		//printf("%d", cursor);
		if((int) strlen(input_buffer) - cursor > 0){
		printf("\033[%dD", (int) strlen(input_buffer) - cursor);//move cursor to correct position
		}
		//printf("%d", cursor);
		fflush(stdout);
	}
	input_buffer[strlen(input_buffer)] = '\0';
	strcpy(command_buffer, input_buffer);
	printf("\n");
	fflush(stdout);
	disable_raw_mode();
}
