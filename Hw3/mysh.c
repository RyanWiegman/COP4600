#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

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

    parse_string = strtok(input_string, " ");
    while (parse_string != NULL) {
        args_array[index] = parse_string;
        parse_string = strtok(NULL, " ");
        index++;
    }
    
    for(int i = 0; i < index; i++){
        printf("token: %s\n", args_array[i]);
    }
    
    return args_array;
}

void launch(char** arguments){
    pid_t pid;
    int status;

    if(arguments == NULL){
        printf("No arguments\n");
        return;
    }

    pid = fork();
    if(pid == 0)
        if(execvp(arguments[0], arguments) == -1){
            printf("shell error");
            perror("shell");
        }
    else if(pid < 0){
        printf("PID error.");
        perror("PID error");
    }
    else
        while(wait(&status) != pid);
}

int main() {
    char** args;

    args = read_line();
    launch(args);
}