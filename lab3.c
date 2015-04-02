#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdbool.h>
#include <string.h>
#include <sys/wait.h>
#include <sys/types.h>

int parseInput(char *command, char **argv);
pid_t wait(int *stat_logic);
pid_t waitpid(pid_t pid, int *stat_loc, int options);
int main()
{
	//input line
	char line[20];

	int status=0;
	int waitStatus;
	//char** temp;
	char *argv[20];
	bool myBool=true;

	while(myBool)
	{
		printf("myShell> ");

		//read in a line
		gets(line);

		//printf("%s",line);

		//parses what the user typed and puts in it argv array
		status=parseInput(line,argv);

		if(strcmp(argv[0],"exit")==0)
		{
			exit(0);
		}

		/*
		pid_t pid=fork();

		if(pid==0)
		{
			if(execvp(argv[0],argv)<0)
			{
				printf("WHOOPS, LOOKS LIKE SOMETHING WENT WRONG HERE\n");
				exit(0);
			}
		}
		*/

		//if we found an & in the command
		//fork and execute the command
		if(status==1)
		{
			pid_t pid=fork();

			if(pid==0)
			{
				if(execvp(argv[0],argv)<0)
				{
					printf("WHOOPS, LOOKS LIKE SOMETHING WENT WRONG HERE\n");
					exit(0);
				}
			}
		}

		//else, we did not find an & in the command
		//still fork and execute the command
		else
		{
			pid_t pid2=fork();

			if(pid2==0)
			{
				execvp(argv[0],argv);
			}

			else
			{
				wait(&waitStatus);
			}
		}
	}
	return 0;
}


//function that parses by tokenizing the command line input
//and returns a pointer to the command so that it can be used later
int parseInput(char *command, char **argv)
{
	int x=0;

	//while we are still reading the input
	while(*command)
	{
		//while we find these characters in the command input,
		//we want to change them to the NULL terminating character
		while(*command == ' ' || *command == '\n' || *command == '\t')
		{
			*command++='\0';
		}

		//save our position within the array that we are putting the 
		//commands in
		*argv++=command;

		//while the command input does not have any of the following characters,
		//want to move command along and check for the &
		while(*command != '\0' && *command != ' ' && *command != '\t' && *command != '\n')
		{
			command++;
			if(*command=='&')
			{
				x=1;
				*command='\0';
			}
		}
	}

	//set *argv to NULL so that we don't end up messing something
	//up with our pointers
	*argv=NULL;
	return x;
}