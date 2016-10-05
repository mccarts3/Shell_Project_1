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
int process_args1(char *argv, char prog[MAX_CMD_CNT][MAX_CMD_LEN], char cmd_args[MAX_CMD_CNT][MAX_ARG_CNT][MAX_ARG_LEN]){
	char delt[ ] = " ";
	int cmd_idx = 0;
	int arg_idx = 0;
	char *tok;
	int  new_cmd = 0;
 
        printf("Input is %s\n",argv); 
	tok=strtok(argv, delt);	
	strcpy(prog[cmd_idx], tok);
	while(tok = strtok(NULL, delt)) {		// Changed strtok(NULL, delt) to strtok(argv, delt)
		if(strcmp(tok, "|") == 0) {
			cmd_args[cmd_idx][arg_idx][0] = (char)0;
			cmd_idx++;
			arg_idx = 0;
			new_cmd = 1;
		} else {
			if(new_cmd == 1 ) {
				strcpy(prog[cmd_idx],tok);
				new_cmd = 0;
			} else{
				strcpy(cmd_args[cmd_idx][arg_idx],tok);
				arg_idx++;
			}
		} 
	}
  
	cmd_args[cmd_idx][arg_idx][0] = (char)0;
	return cmd_idx;
}



int  process_args( char *argv, char prog[MAX_CMD_CNT][MAX_CMD_LEN], char cmd_args[MAX_CMD_CNT][MAX_ARG_CNT][MAX_ARG_LEN]){

   //Todo
   char delt[ ] = " ";
   int cmd_idx = 0;
   int arg_idx = 0;
   char *tok;
   int  new_cmd = 0;

   tok=strtok( argv, delt);
   strcpy(prog[cmd_idx],tok);
   while( tok = strtok( NULL,delt) ){
     if( strcmp(tok, "|") == 0 ){
          cmd_args[cmd_idx][arg_idx][0]=(char)0;
          cmd_idx++;
          arg_idx = 0;
          new_cmd = 1;
      }else{
         if( new_cmd == 1 ){
             strcpy(prog[cmd_idx],tok);
             new_cmd = 0;
         }else{
                strcpy(cmd_args[cmd_idx][arg_idx],tok);
                arg_idx++;
        }
     }
   }

  cmd_args[cmd_idx][arg_idx][0]=(char)0;
  return cmd_idx;
}



void exec_cmd_pipe(char *cmd, char *args, int fd[]) {
    dup2(fd[1] ,1);	// redirect output	
    dup2(fd[0] ,0);	// redirect input

    //Replace this later to handle cmd and args
    // char *args;
    // scanf(" %d", &args);
    char *args1[ ] = {"ls", "ls", "-l", NULL};
    char *cmd1 = "ls";
    execvp(cmd1, args1);
}


void main( int argc, char *argv[ ] ) {
	int cmd_cnt = 0;
	int pid = 0;
	int child_pid[MAX_CMD_CNT];
	int i, status;
	int fd[2];
        char inputs[100];

     strcpy(inputs, argv[1]);
     printf("argv is %s\n",argv[1]);  	
     cmd_cnt = process_args( inputs, cmds, cmd_args);
	if( cmd_cnt == 0 ) {
		printf("fail to get user input\n");
		return;
	}

	//Create the pipe
	pipe(fd);	// Use the file descriptors (fd[]) later for exec_cmd_pipe

	//fork a child process for each cmd
	for( i = 0; i < cmd_cnt; i++ ) {
		pid = fork();
		if( pid == 0 ) {    //Child process
		  //exec_cmd_pipe(char *cmd, char *args, int fd[])
		  exec_cmd_pipe(cmds[i], cmd_args[i], fd);
		}else {             //Parent process
		  child_pid[i] = pid;
		}
	}

	for( i = 0 ; i < cmd_cnt; i++) {
		pid = wait( &status);
		printf("Child process %d is done\n", pid);
	}

	printf("Shell is done. \n" );
}
