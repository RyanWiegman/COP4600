#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

#define DELIMITER " \n"

char** read_line() {
    char input_string[1024];
    char* parse_string = malloc(sizeof(char));
    char** args_array = malloc(sizeof(char*));
    int index = 0;

    if(parse_string == NULL || args_array == NULL){
        printf("Malloc error.\n");
        exit(EXIT_FAILURE);
    }

    fgets(input_string, 1024, stdin);
    input_string[strlen(input_string) - 1] = '\0';

    parse_string = strtok(input_string, DELIMITER);
    while (parse_string != NULL) {
        args_array[index] = parse_string;
        parse_string = strtok(NULL, DELIMITER);
        index++;
    }
    
    for(int i = 0; i < index; i++){
        printf("token: %s\n", args_array[i]);
    }
    return args_array;
}

int launch(char** args){
    pid_t pid;
    int status;

    if(args == NULL){
        printf("No arguments\n");
        return 1;
    }

    pid = fork();
    if(pid == 0) {
        printf("\nInside child first token: %s\n", args[0]);
        if(execvp(args[0], args) == -1){
            printf("shell error");
            perror("shell");
        }
    }
    else if(pid < 0){
        printf("PID error.");
        perror("PID error");
    }
    else
        while(wait(&status) != pid);
    return 1;
}

int main() {
    char** args;
    int run = 1;

    while(run){
        args = read_line();
        run = launch(args);
        printf("run var: %d\n", run);
    }
   return EXIT_SUCCESS;
}