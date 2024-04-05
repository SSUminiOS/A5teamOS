#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <readline/readline.h>
#include <readline/history.h>
#include<unistd.h>


#include "system.h"

#define MAX_ARGS 10
void print_minios(char* str);

int main() {
    print_minios("[MiniOS SSU] Hello, World!");
    int pid=fork();
    if(pid==-1) return-1;
    else if(pid==0){SHM();}
    else{
    char *input;
    char *command; 
    char *args[20];
    int num_args = 0;
    minimkroot();
    
    
    
    while(1) {
        // readline을 사용하여 입력 받기
        input = readline("커맨드를 입력하세요(종료:exit) : ");
	command = strtok(input," ");
	while((args[num_args] = strtok(NULL," ")) !=NULL &&num_args<MAX_ARGS -1)
		num_args++;
	
	
        if (strcmp(command,"exit") == 0) {
            break;
        }

        else if (strcmp(command,"minisystem") == 0){
            minisystem();
        }
        else if (strcmp(command,"minicd") == 0){

            minicd(args[0]);
        }
        else if (strcmp(command,"minils") == 0){
            minils();
        }
        else if (strcmp(command,"minimkdir") == 0){
                	int i;
        	for(i=0;i<num_args;i++){
            minimkdir(args[i]);
           }
            printf("Directory created\n");
        }
        else if (strcmp(command,"minirmdir") == 0){
                	int i;
        	for(i=0;i<num_args;i++){
            minirmdir(args[i]);}
            printf("Directory deleted\n");
        }
        else system(input);
        
        num_args=0;
    }

    // 메모리 해제
    free(input);
    print_minios("[MiniOS SSU] MiniOS Shutdown........");

    return(1);}
}

void print_minios(char* str) {
        printf("%s\n",str);
}
