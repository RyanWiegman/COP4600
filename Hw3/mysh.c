#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void read_line(char cmd[], char *parameter[]) {
    char input_string[1024];
    char *parse_string, *arg_array[1024];
    int index = 0;

    fgets(input_string, 1024, stdin);

    parse_string = strtok(input_string, " ");
    while (parse_string != NULL) {
        arg_array[index++] = parse_string;
        parse_string = strtok(NULL, " ");
    }
    
    printf("Token: %s\n", arg_array[0]);
    printf("Token: %s", arg_array[1]);

    
}

int main() {
    char cmd[50], *parameter[100];

    read_line(cmd, parameter);
}