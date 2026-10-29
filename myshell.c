/* CMSC-520: Operating Systems
 * Project 1: A My Shell Program
 * Professor: Dr. Lee
 * File Created: 2026.10.13
 * File Updated: 2026.10.17
 */

#include "stdio.h"                
#include <unistd.h>              
#include <string.h>               
#include <stdlib.h>               
#include <sys/types.h>           
#include <sys/wait.h>             

int MAX_NUM_CMDS = 10;            // Maximum number of parsed arguments allowed per command
int MAX_CMD_LENGTH = 100;         // Maximum character length allowed for input strings
int MAX_SIZE_DIR = 1024;          // Maximum size for directory paths

void *get_str_input(char *input){
    fgets(input, MAX_CMD_LENGTH, stdin);    // Reads input string up to max length into buffer
    
    input[strcspn(input, "\r\n")] = '\0'; // Replaces newline and carriage chars with null terminator
}

void parse_input(char *input, char *commands[]){
    const char del[] = " ";                 // Defines the space char as the delimiter
    char *token = strtok(input, del);       // Gets the first token from the input string
    int counter = 0;                        // Initializes counter as 0
    
    while(token != NULL && counter < MAX_NUM_CMDS){ // Loops while tokens exist and stay within bounds
        commands[counter++] = token;        // Stores current token and increments counter
        token = strtok(NULL, del);          // Gets the next token from the input string
    }
    
    commands[counter] = NULL;               // Null-terminates the command argument array for execution
}

void run_pause(){
    printf("Press [Enter] to continue...");   // Prompts the user to press the enter key
    getchar();                              // Waits for user keyboard input
}

void echo(char **ptr){
    int counter = 0;                        // Initializes counter as 0
    while(ptr[counter] != NULL){            // Loops through each argument until the null terminator
        printf("%s ", ptr[counter++]);      // Prints each argument followed by a space
    }
    printf("\n");                           // Prints a final newline after echoing all arguments
}

void cd(char *dir){
    if(dir == NULL){                        // Checks if there is no target directory
        char cwd[MAX_SIZE_DIR];             // Declares buffer for current working directory
        if(getcwd(cwd, sizeof(cwd)) != NULL){ // Retrieves current working directory successfully
            printf("%s\n", cwd);            // Prints current working directory
        }
        else{
            printf("Error: cannot get current working directory.\n"); // Prints error 
        }
    }
    else{
        if(chdir(dir) == 0){                // Attempts to change working directory
            char new_cwd[MAX_SIZE_DIR];     // Declares buffer for new working directory
            if(getcwd(new_cwd, sizeof(new_cwd)) != NULL){ // Retrieves new working directory
                setenv("PWD", new_cwd, 1);  // Updates the PWD environment variable with the new path
            }
        }
        else{
            printf("Error: directory could not be found.\n"); // Prints error
        }
    }
}

void environment_str(){
    extern char **environ;                  // Accesses the global environment variables pointer array
    int counter = 0;                        // Initializes counter to 0
    
    while(environ[counter] != NULL){        // Loops through all available environment strings
        printf("%s\n", environ[counter++]); // Prints each environment string     }
    }
}

void dir(char *dir){
    char command[MAX_CMD_LENGTH];           // Declares buffer to hold directory
    
    if (dir == NULL) {                      // Checks if there is no target directory
        snprintf(command, sizeof(command), "ls"); // Puts "ls" into buffer
    } else {
        snprintf(command, sizeof(command), "ls %s", dir); // Put "ls directory" into buffer
    }
    
    if(system(command) != 0){               // Lists directory using system call
        printf("Error: directory could not be listed or found.\n"); // Prints error
    }
}

int main(int argc, char *argv[]) {
    
    const char *relative_path = argv[0];    // Captures how the shell was executed
    char *absolute_path = realpath(relative_path, NULL); // Stores the absolute path based on the relative path
    
    if(absolute_path != NULL){
        setenv("shell", absolute_path, 0);  // Sets the shell environment variable to absolute path
    }
    else{
        printf("Error: unable to set environment variable shell."); // Prints error
    }
    
    while(1) {                              // While the user wants to continue

        char cwd[MAX_SIZE_DIR];             // Buffer for current directory 
        char input[MAX_CMD_LENGTH];         // Buffer for user input
        char *commands[MAX_NUM_CMDS];       // Array to hold parsed command arguments
        
        memset(commands, 0, sizeof(commands)); // Clears command array to insure no leftover values from last loop
        if(getcwd(cwd, sizeof(cwd)) != NULL){ // Gets current directory for prompt display
            printf("%s>", cwd);             // Prints current directory
            get_str_input(input);           // Reads user input
            parse_input(input, commands);   // Parses input string into tokens
        }
        else{
          printf("Directory error. Cannot proceed.\n"); // Exits if failure
          return 1;
        }
        
        char *command = commands[0];        // Assigns base command from first command token
    
        if(command == NULL){
            continue;                       // Skips if user entered nothing
        }
        else if(strcmp(command, "mycd") == 0){
            if(commands[2] != NULL) {
                printf("Error: mycd contains too many arguments.\n"); // Validates argument count
            }
            else{
               cd(commands[1]);             // Uses cd() function to execute mycd
            }
        }
        else if(strcmp(command, "myclr") == 0){
            if(commands[1] != NULL){
                printf("Error: myclr contains too many arguments.\n"); // Validates argument count
                continue;
            }
            else{
                system("clear");            // Clears screen using system call
            }
        }
        else if(strcmp(command, "mydir") == 0){
            if(commands[2] != NULL){
                printf("Error: mydir contains too many arguments.\n"); // Validates argument count
                continue;
            }
            else{
                dir(commands[1]);           // Uses dir() function to execute mydir
            }
        }
        else if(strcmp(command, "myenviron") == 0){
            if(commands[1] != NULL){
                printf("Error: myenviron contains too many arguments.\n"); // Validates argument count
                continue;
            }
            else{
                environment_str();          // Uses enviornment_str() function to execute myenviron
            }
        }
        else if(strcmp(command, "myecho") == 0){
            if(commands[1] == NULL){
                printf("Error: <comment> cannot be empty\n"); // Prints error on missing <comment>
                continue;
            }
            else{
                char **comment = commands + 1; // Creates pointer to start at the next input token which is the start of <comment>
                echo(comment);              // Uses echo() function to execute myecho
            }
        }
        else if(strcmp(command, "myhelp") == 0){
            if(commands[1] != NULL){
                printf("Error: myhelp contains too many arguments.\n"); // Validates argument count
                continue;
            }
            else{
                system("more readme");      // Uses system call to display readme with more tag
            }
        }
        else if(strcmp(command, "mypause") == 0){
            if(commands[1] != NULL){
                printf("Error: mypause contains too many arguments.\n"); // Validates argument count
                continue;
            }
            else{
                run_pause();                // Uses run_pause() function to execute mypause
            }
        }
        else if(strcmp(command, "myquit") == 0){
            if(commands[1] != NULL){
                printf("Error: myquit contains too many arguments.\n"); // Validates argument count
                continue;
            }
            else{
                return 0;                   // Exits program.
            }
        }
        else{
            
            pid_t pid;                      // Declares process ID
            pid = fork();                   // Fork shell into parent and child
            
            if(pid < 0){
                printf("Error: fork failed."); // If process creation fails, print error
                continue;
            }
            else if(pid == 0){
                
                if(setenv("parent", getenv("shell"), 0) != 0) { // Sets parent environment variable for child
                    printf("Error: unable to set environment variable parent."); // Print error
                    exit(1);                // Exit child shell
                }
                
                execvp(commands[0], commands); // Replaces child process with external command
		exit(0);
                
            }
            else {
                wait(NULL);                 // Pauses parent shell until child finishes
            }
        }
    }
}
