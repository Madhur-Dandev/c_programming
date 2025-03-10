#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>

void print_error(char *, char *);
short check_arg(char *);
void print_help(void);

int main(int argc, char *argv[])
{
	char source[255], destination[255];
	if((argc == 2 || argc == 3) && argv[1][0] == '-')
	{
		short arg = check_arg(argv[1]);
		if(arg == 2)
		{
			print_help();
			return 0;
		}

		if(arg == -1)
		{
			print_error("Argument error:", "Invalid argument type.\nType: \"make_copy.exe -h\" for help");
			return EXIT_FAILURE;
		}
	}
	else if(argc == 3 && argv[1][0] != '-' && argv[2][0] != '-')
	{
		strcpy(source, (const char *) argv[1]);
		strcpy(destination, (const char *) argv[2]);
	}
	else if(argc == 5 && argv[1][0] == '-' && argv[3][0] == '-')
	{
		short first_arg = check_arg(argv[1]);
		short second_arg = check_arg(argv[3]);
		if(first_arg == 0 && second_arg == 1)
		{
			strcpy(source, argv[2]);
			strcpy(destination, argv[4]);
		}
		else if(first_arg == 1 && second_arg == 0)
		{
			strcpy(destination, argv[2]);
			strcpy(source, argv[4]);
		}
		else
		{
			print_error("Argument error:", "Invalid argument type.\nType: \"make_copy.exe -h\" for help");
			return EXIT_FAILURE;
		}

	}
	else if(argc == 4 && argv[1][0] == '-' && argv[3][0] != '-')
	{
		short arg = check_arg(argv[1]);
		if(arg == 0)
		{
			strcpy(source, argv[2]);
			strcpy(destination, argv[3]);
		}
		else if(arg == 1)
		{
			strcpy(destination, argv[2]);
			strcpy(source, argv[3]);
		}
		else
		{
			print_error("Argument error:", "Invalid argument type.\nType: \"make_copy.exe -h\" for help");
			return EXIT_FAILURE;
		}
	}
	else if(argc == 4 && argv[1][0] != '-' && argv[2][0] == '-')
	{
		short arg = check_arg(argv[2]);
		if(arg == 0)
		{
			strcpy(source, argv[3]);
			strcpy(destination, argv[1]);
		}
		else if(arg == 1)
		{
			strcpy(destination, argv[3]);
			strcpy(source, argv[1]);
		}
		else
		{
			print_error("Argument error:", "Invalid argument type.\nType: make_copy.exe -h for help");
			return EXIT_FAILURE;
		}
	}
	else
	{
		print_error("Argument error:", "Invalid number of argument");
		return EXIT_FAILURE;
	}

	//printf("%s %s\n", source, destination);
	FILE *sp, *dp;
	sp = fopen(source, "r");
	dp = fopen(destination, "w");

	if(!sp || !dp)
	{
		perror("fopen() failed");
		if(sp)
			fclose(sp);
		if(dp)
			fclose(dp);
		return errno; 
	}

	char buffer[32];
	while(fgets(buffer, 32, sp))
		fputs(buffer, dp);

	fclose(sp);
	fclose(dp);
	puts("File is copied.");
	return 0;
}

void print_error(char *hl, char *msg)
{
	printf("%s: %s\n", hl, msg);
	return;
}

short check_arg(char *arg)
{
	short len = strlen(arg);
	if(len > 2 || len < 2)
		return -1;

	switch(arg[1])
	{
		case 's':
			return 0;
		case 'd':
			return 1;
		case 'h':
			return 2;
		default:
			return -1;
	}
}

void print_help(void)
{
	printf("-s: for source\n-d: for destination\nType: make_copy.exe source destination\n");
}
