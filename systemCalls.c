#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <unistd.h>
#include <fcntl.h>
#include <errno.h>
#include <sys/stat.h>

#define MAX_INPUT 1000
#define MAX_TOKEN_SIZE 100
#define MAX_NUM_TOKENS 100

void parseInput(char *input, char *splitWords[]){
	int wordInd = 0;
	splitWords[0] = strtok(input, " \n");
	while(splitWords[wordInd] != NULL){
		splitWords[++wordInd] = strtok(NULL, " \n");
	}
}

int executeCommand(char *args[]){
	pid_t pid = fork();
	if(pid == -1){
		printf("fork failed!\n");
		return -1;
	}else if(pid == 0){
		if(execvp(args[0], args) == -1){
			perror("fork failed!");
			return -1;
		}
		exit(EXIT_FAILURE);
	}else{
		int status;
		waitpid(pid, &status, 0);
		if(WIFEXITED(status) && !WEXITSTATUS(status)){
			return 0;
		}
		return 1;
	}
}

void changeDirectories(char *arg){
	if(arg == NULL || strlen(arg) == 0){
		printf("Path Not Found!\n");
		return;
	}
	if(chdir(arg) == -1){
		printf("Path Not Found!\n");
	}
}

int main(){
	char input[MAX_INPUT];
	char *splitWords[MAX_NUM_TOKENS];
	char *prompt = "exampleShell-lvidetto$ ";
	
	while(1){
		printf("%s", prompt);
		fgets(input, MAX_INPUT, stdin);
		parseInput(input, splitWords);
		
		if(splitWords[0] == NULL){
			continue;
		}
		
		if(strcmp(splitWords[0], "exit") == 0){
			return 0;
		}
		else if(strcmp(splitWords[0], "cd") == 0){
			changeDirectories(splitWords[1]);
		} else{
			if(executeCommand(splitWords) > 0){
				printf("Command failed!\n");
			}
		}
		
		memset(input, 0, sizeof(input));
		memset(splitWords, 0, sizeof(splitWords));
	}
	
	return 0;
}
