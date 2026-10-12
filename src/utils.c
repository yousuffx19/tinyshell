#include <unistd.h>
#include <string.h>

int set_working_directory(char *command){
	if(command[0] == 'c' && command[1] == 'd'){
		if(chdir(command+3) == -1){
			return -1;
		}
		return 1;
	}
	return 0;
}
