#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

#define DELIMITER " \n"
#define BUFFER 1024

char* currentdir;

void set_dir(){
    char cwd[BUFFER];
    getcwd(cwd, sizeof(cwd));
    strcpy(currentdir, cwd);
}

void print_cwd() {
    printf("inside print_cwd\n\n");
    printf("%s\n", currentdir);
    fflush(stdout);
}

void change_dir(char** args){
    printf("inside change_dir.\n\n");
    char* test_dir = malloc(sizeof(char) * BUFFER);
    int result;

    strcpy(test_dir, currentdir);
    strcat(test_dir, "/");
    strcat(test_dir, args[1]);

    printf("print current dir after strcat: %s\n", currentdir);
    printf("after strcat for test_dir: %s\n", test_dir);
    result = access(test_dir, F_OK);
    if(!result){
        printf("inside result statement.\n");
        currentdir = test_dir;
        printf("After new current_dir is saved: %s", currentdir);
    }
    else{
        printf("Directory does not exist.\n");
    }
    //free(test_dir);   dont know why but this clears currentdir as well.
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
    printf("inside check_builtins\n\n");

    if(strcmp(args[0], "movetodir") == 0)
        change_dir(args);
    else if(strcmp(args[0], "whereami") == 0)
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
    char input_string[BUFFER];
    char* parse_string = malloc(sizeof(char) * BUFFER);
    char** args_array = malloc(sizeof(char*) * BUFFER);
    int index = 0;
    FILE *intext = fopen("args_history.txt", "a");

    if(parse_string == NULL || args_array == NULL){
        printf("Malloc error.\n");
        exit(EXIT_FAILURE);
    }
    if(parse_string)

    fgets(input_string, BUFFER, stdin);
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
    printf("give me something anything");
    return args_array;
}

int main() {
    currentdir = malloc(sizeof(char) * BUFFER);
    char** args;
    //char** args_history;
    //char* currentdir;
    int run = 1, clear = 1;
    set_dir(currentdir);

    while(run){
        if(clear){
            system("clear");
            clear = 0;
        }
        printf("%s", currentdir);
        printf("#");

        args = read_line();
        printf("Before check_builtin call. \n");
        run = check_builtin(args);
        //run = launch(args);
        printf("\n\n\nrun var: %d\n", run);
        free(args);
    }
   return EXIT_SUCCESS;
}