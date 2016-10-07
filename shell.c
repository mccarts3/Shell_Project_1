#include<stdlib.h>
#include<stdio.h>
#include<unistd.h>
#include<sys/wait.h>
#include<string.h>

#define  MAX_CMD_CNT 100
#define  MAX_CMD_LEN 40
#define  MAX_ARG_CNT 10
#define  MAX_ARG_LEN 20


char cmds[MAX_CMD_CNT][MAX_CMD_LEN];
char cmd_args[MAX_CMD_CNT][MAX_ARG_CNT][MAX_ARG_LEN];

int pipes[MAX_CMD_CNT][2];	//Not used


/*
	input:
		argv holds the input string from user.
		Mulitple  cmds are delimited by the | in the input string. 
	output:
		prog[ ]   holds each cmd in the input string.
		cmd_args  holds args for each cmd
	return value:
		0:     error
		>=1:   number of cmds in the input

*/
int  process_args(char *argv, char prog[MAX_CMD_CNT][MAX_CMD_LEN], char cmd_args[MAX_CMD_CNT][MAX_ARG_CNT][MAX_ARG_LEN]){
    char delt[ ] = " ";
    int cmd_idx = 0;
    int arg_idx = 0;
    char *tok;
    int  new_cmd = 0;
    
    // tok is a char* that goes from the first character to the first space
    //  - most likely, will be "ls"
    tok = strtok(argv, delt);
    strcpy(prog[cmd_idx], tok);     // Copies "ls" to prog[0] (prog[] holds the commands)
    
    while((tok = strtok(NULL, delt))) {
        // If the pipe character is the next "string"...
        if (strcmp(tok, "^") == 0) {
            // Place a '0' at the next argument for the current command
            cmd_args[cmd_idx][arg_idx][0] = (char)0;
            cmd_idx++;      // Move onto next command after the pipe
            arg_idx = 0;    // Add arguments at the 0th position now
            new_cmd = 1;
        } else {
            // New command, add next string token to prog[]
            if(new_cmd == 1) {
                strcpy(prog[cmd_idx], tok);
                new_cmd = 0;
            } else {
                // Not a command, so must be an argument
                strcpy(cmd_args[cmd_idx][arg_idx],tok);
                arg_idx++;
            }
        }
    }
    
    // Add a 0 to the end of the current cmd argument to signal the end
    cmd_args[cmd_idx][arg_idx][0] = (char)0;
    return cmd_idx;
}

void exec_cmd_pipe(char *cmd, char *args, int fd[]) {
    dup2(fd[0], 0);     // redirect input using 1 (equal to constant STDIN_FILENO
    dup2(fd[1], 1);     // redirect output using 0 (equal to constant STDOUT_FILENO
    
    char *arguments[] = {cmd, args, NULL};
    
    // execvp(cmd, args)
    //  - cmd is a "string" that contains the name of file to be executed
    //  - args is a char**
    execvp(cmd, arguments);
}


int main( int argc, char *argv[ ] ) {
    int cmd_cnt = 0;
    int pid = 0;
    int child_pid[MAX_CMD_CNT];
    int i, status;
    int fd[2];
    char inputs[100];

    strcpy(inputs, argv[1]);
    cmd_cnt = process_args(inputs, cmds, cmd_args);
    
    // Display the commands and arguments
    // Used for debugging
//    for(i = 0; i <= cmd_cnt; i++) {
//        printf("The %d command is: ", i);
//        printf("%s\n", cmds[i]);
//        int arg_index = 0;
//        char *curr_arg = cmd_args[i][arg_index];
//        
//        while (curr_arg[0] != (char)0) {
//            printf("The argument is: %s\n", curr_arg);
//            arg_index++;
//            curr_arg = cmd_args[i][arg_index];
//        }
//    }
    
    if( cmd_cnt == 0 ) {
		printf("fail to get user input\n");
		return -1;
	}

	//Create the pipe
	pipe(fd);	// Use the file descriptors (fd[]) later for exec_cmd_pipe

	//fork a child process for each cmd
	for(i = 0; i <= cmd_cnt; i++) {
		pid = fork();
		if(pid == 0) {      //Child process
		  exec_cmd_pipe(cmds[i], cmd_args[i], fd);
		} else {             //Parent process
		  child_pid[i] = pid;
		}
	}

	for( i = 0 ; i < cmd_cnt; i++) {
		pid = wait(&status);
		printf("Child process %d is done\n", pid);
	}

	printf("Shell is done. \n" );
    return 0;
}
