Project 1 Notes
----------------

Goals:
 -	run ./shell
 -	In this program, run commands (like "./sum 12 | ./sum 46" or "ls -l | wc -c")
 -	Have ./shell determine the commands and the arguments
 -	Fork each argument for the child to execute and pipe the results as declared above
 -	Output the results in the parent process of ./shell
 
Confusion:
 - 	Should I be using shell.c, proc_args.c, or both?
 -	Should I be able to run test.c within the shell program while it is being run?
 
In shell.c
 - 	main
	-	Get the cmd_cnt (COMPLETED)
	-	Create pipes (how?? Output to terminal/shell/screen?)
	-	For each cmd, fork a child process to handle it
		-	call exec_cmd_pipe with the cmd and the pipe to handle it
	-	Have the parent process wait for the child process to finish, then print stuff (COMPLETED)
 - 	process_args 
	-	handle the char *argv[ ] input ("./sum 12 | ./sum 46", etc)
	-	put the "./sum" or "ls" parts of *argv[ ] into char *prog[ ]
	-	put the "12", "46", "-l", or "-c" parts of *argv[ ] into char ***cmd_args
	-	***IS THIS COMPLETED ALREADY?***
 -	exec_cmd_pipe
	-	Use dup2 to redirect the child process input/output to the pipe so that the process does not need to worry about where the data is inputting/outputting from.  This will now be handled by the pipe