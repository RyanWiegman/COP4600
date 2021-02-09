#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

#define DELIMITER " \n"

void print_cwd() {
    char cwd[1024];
    printf("%s\n", getcwd(cwd, sizeof(cwd)));
}

void print_history() {
    char* line = NULL;
    size_t buf_size = 0;
    ssize_t line_count;
    int counter = 0;
    FILE* out_text = fopen("args_history.txt", "r");

    line_count = getline(&line, &buf_size, out_text);
    while (line_count >= 0){
        printf("%d: %s",counter++, line);
        line_count = getline(&line, &buf_size, out_text);
    }
    fclose(out_text);
}

void clear_history(){
    FILE* clear = fopen("args_history.txt", "w");
    fclose(clear);
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

int check_builtin(char** args) {
   
    if(strcmp(args[0], "whereami") == 0)
        print_cwd();
    else if(strcmp(args[0], "history") == 0){
        if(args[1] != NULL && strcmp(args[1], "-c") == 0)
            clear_history();
        else
            print_history();
        return 1;
    }
    else if(strcmp(args[0], "byebye") == 0)
        return EXIT_SUCCESS;
    return 1;
}

char** read_line() {
    char input_string[1024];
    char* parse_string = malloc(sizeof(char));
    char** args_array = malloc(sizeof(char*));
    int index = 0;
    FILE *intext = fopen("args_history.txt", "a");

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
        fprintf(intext, "%s\n", args_array[i]);
    }
    fclose(intext);
    return args_array;
}

int main() {
    char** args;
    char** args_history;
    int run = 1, clear = 1;

    while(run){
        if(clear){
            system("clear");
            clear = 0;
        }
        printf("#");

        args = read_line();
        run = check_builtin(args);
        //run = launch(args);
        printf("\nrun var: %d\n", run);
    }
   return EXIT_SUCCESS;
}