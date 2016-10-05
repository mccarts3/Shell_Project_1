#include<stdlib.h>
#include<stdio.h>
#include<string.h>


#define MAX_CMD_CNT  10
#define MAX_CMD_LEN  20
#define MAX_ARG_CNT  10
#define MAX_ARG_LEN  20


char cmds[MAX_CMD_CNT][MAX_CMD_LEN];
char cmd_args[MAX_CMD_CNT][MAX_ARG_CNT][MAX_ARG_LEN];

/*
   input:
       argv holds the input string from user.
       Mulitple  cmds are delimited by the | in the
       input string.
   output:
       prog[ ]   holds each cmd in the input string.
       cmd_args  holds args for each cmd
   return value:
       0:     error
       >=1:   number of cmds in the input

*/
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

void main( ){

char inputs[]=" ls -l | wc -c";
int cnt = 0;

 cnt = process_args( inputs, cmds, cmd_args);

  //test
 for( int i = 0 ; i <= cnt ; i++ ){
   printf("The %dth cmd is %s   ",i,cmds[i]);
   printf(" args: ");
   for( int j = 0; cmd_args[i][j][0] != 0; j++)
     printf(" %s ",cmd_args[i][j]);
   printf("\n");
 }
   

 }



