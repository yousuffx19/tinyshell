#include <unistd.h>
#include <string.h>


int strip_end_space(char* full_string){
	int n = (int) strlen(full_string);
	n--;
	while(full_string[n] == ' '){
		n--;
	}
	n++;
	full_string[n] = '\0';
       	return n;	
}

int strip_start_space(char* full_string){
	int p = 0;
	while(full_string[p] == ' '){
		p++;
	} 
	return p;
}

int set_working_directory(char *command){
	if(command[0] == 'c' && command[1] == 'd'){
		if(chdir(command+3) == -1){
			return -1;
		}
		return 1;
	}
	return 0;
}
