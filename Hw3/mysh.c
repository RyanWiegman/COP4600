#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

#define DELIMITER " \n"
#define BUFFER 1024

int check_builtin(char**);
char* currentdir;

void set_dir(){
    char cwd[BUFFER];
    getcwd(cwd, sizeof(cwd));
    strcpy(currentdir, cwd);
}

void print_cwd() {
    printf("%s\n", currentdir);
}

void change_dir(char** args){
    //printf("inside change_dir.\n\n");
    char* test_dir = malloc(sizeof(char) * BUFFER);
    int result;

    if(args[1] == NULL || test_dir == NULL)
        return;

    strcpy(test_dir, currentdir);
    strcat(test_dir, "/");
    strcat(test_dir, args[1]);

    result = access(test_dir, F_OK);
    if(!result){
        currentdir = test_dir;
    }
    else{
        printf("Directory does not exist.\n");
    }
}

void clear_history(){
    FILE* clear = fopen("args_history.txt", "w");
    fclose(clear);
}

void print_history(char** args) {
    //printf("Inside print history.\n\n");

    if(args[0] == NULL)
        return;

    if(args[1] != NULL){
        if(strcmp(args[1], "-c") == 0)
            clear_history();
    }
    else{
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
}

char* check_path_type(char* arg) {
    //printf("Inside check path type.\n\n");

    char* path = arg;

    if(path[0] == '/')
        return arg;
    else {
       char* new_path = malloc(sizeof(char) * BUFFER);
       strcpy(new_path, currentdir);
       strcat(new_path, "/");
       strcat(new_path, path);
       return new_path;
    }    
}

int start(char** args){
    //printf("Inside start: \n\n");

    pid_t pid;
    int status;

    if(args == NULL){
        printf("No arguments\n");
        return 1;
    }

    pid = fork();
    if(pid == 0) 
        if(execvp(args[0], args) == -1)
            perror("shell");
    else if(pid < 0)
        perror("PID error\n");
    else
        while(wait(&status) != pid);
    return 1;
}

void replay(char** args) {
    //printf("inside replay.\n");

    FILE* read = fopen("args_history.txt", "r");
    size_t buf_size = 0;
    ssize_t line_count;
    int line_number = atoi(args[1]);
    int counter = 0, index = 0, run = 0;
    char* args_line = NULL;
    char** selected_args = malloc(sizeof(char*) * BUFFER);

    line_count = getline(&args_line, &buf_size, read);
    while (line_count >= 0){
        if(counter == line_number){
            args_line[strlen(args_line)] = '\0';
            args_line = strtok(args_line, DELIMITER);

            while(args_line != NULL){
                selected_args[index] = args_line;
                args_line = strtok(NULL, DELIMITER);
                index++;
            }
            selected_args[index] = NULL;
            run = check_builtin(selected_args);
        }
        counter++;
        line_count = getline(&args_line, &buf_size, read);
    }

    if(line_number > counter)
        printf("command does not exist.\n");
    free(selected_args);
    fclose(read);
}

int background(char** args) {
    //printf("Inside background: \n\n");

    pid_t pid;
    int status;

    if(args[0] == NULL){
        printf("No arguments.\n");
        return 1;
    }
    printf("PID: %d\n", pid);

    int index = 0;
    while(args[index + 1] != NULL){
        args[index] = args[index + 1];
        index++;
    }
    args[index] = NULL;
    args[0] = check_path_type(args[0]);

    pid = fork();
    
    if(pid == 0) 
        if(execvp(args[0], args) == -1)
            perror("shell");
    else if(pid < 0)
        perror("PID error\n");
    else
        while(wait(&status) != pid);   
    return 0;
}

void kill_program(char** args) {
    //printf("Insider kill program.\n\n");

    int pid_number = atoi(args[1]);
    kill(pid_number, SIGKILL);
}

int check_builtin(char** args) {
    //printf("inside check_builtins\n\n");
    if(args == NULL)
        return 1;

    else if(strcmp(args[0], "movetodir") == 0)
        change_dir(args);
    else if(strcmp(args[0], "whereami") == 0)
        print_cwd();
    else if(strcmp(args[0], "history") == 0)
        print_history(args);
    else if(strcmp(args[0], "byebye") == 0)
        exit(1);
    else if (strcmp(args[0], "replay") == 0)
        replay(args);
    else if(strcmp(args[0], "start") == 0){
        int index = 0;
        while(args[index + 1] != NULL){
            args[index] = args[index + 1];
            index++;
        }
        if(index != 0)
            args[index] = NULL;

        args[0] = check_path_type(args[0]);
        start(args);
    }    
    else if(strcmp(args[0], "background") == 0)
        background(args);
    else if(strcmp(args[0], "dalek") == 0)
        kill_program(args);
    else
        return 0;
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

    fgets(input_string, BUFFER, stdin);
    input_string[strlen(input_string) - 1] = '\0';

    parse_string = strtok(input_string, DELIMITER);
    while (parse_string != NULL) {
        args_array[index] = parse_string;
        parse_string = strtok(NULL, DELIMITER);
        index++;
    }
    args_array[index] = NULL;

    for(int i = 0; i < index; i++){
        if(i > 0 && strcmp(args_array[0], "movetodir") == 0)
            fprintf(intext, "/");
        fprintf(intext, "%s ", args_array[i]);
    }
    fprintf(intext, "\n");
    fclose(intext);
    free(parse_string);
    return args_array;
}

int main() {
    currentdir = malloc(sizeof(char) * BUFFER);
    char** args;
    int run = 1, clear = 1;
    set_dir(currentdir);

    while(run){
        if(clear){
            system("clear");
            clear = 0;
        }
        printf("#");

        args = read_line();
        run = check_builtin(args);
        if(!run && !strcmp(args[0], "byebye") == 0)
            run = start(args);
        free(args);
    }
   return EXIT_SUCCESS;
}