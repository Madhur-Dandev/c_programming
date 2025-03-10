#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <sys/stat.h>

int main(void)
{
	// reading
	/*FILE *fp;
	char *file_destination = "../dsa/tree/binary_tree.hh";
	fp = fopen(file_destination, "r");

	if(!fp)
	{
		perror("fopen() failed");
		return errno;
	}

	char buffer[8];
	while(fgets(buffer, 8, fp))
		fputs(buffer, stdout);
	fputc('\n', stdout);
	fclose(fp);*/

	// writing
	/*FILE *fp;
	char *destination = "./sample.txt";
	fp = fopen(destination, "w");
	
	if(!fp)
	{
		perror("fopen() failed");
		return errno;
	}

	fputs("Hello I/O", fp);
	fflush(fp);
	fclose(fp);*/

	// file removal
	/*char *file = "sampletxt";

	if(remove(file))
		printf("%d\n", errno);*/

	// copying content to another file
	/*FILE *sp, *dp;
	char *source = "../dsa/tree/binary_tree.c";
	char *destination = "../dsa/tree/binary_tree_backup.c";
	sp = fopen(source, "r");
	dp = fopen(destination, "w");

	if(!sp || !dp)
	{
		perror("fopen() failed");
		return errno;
	}

	char buffer[16];

	while(fgets(buffer, 16, sp))
		fputs(buffer, dp);

	fclose(sp);
	fclose(dp);*/

	// renaming file
	/*FILE *reading, *new_file;
	char *source = "./temp.txt";
	char *new_name = "./sample.txt";
	
	reading = fopen(source, "r");
	new_file = fopen(new_name, "w");

	if(!reading || !new_file)
	{
		perror("fopen() failed");
		return errno;
	}

	char buffer[32];
	while(fgets(buffer, 32, reading))
	{
		fputs(buffer, new_file);
		fflush(new_file);
	}
	
	remove(source);
	fclose(reading);
	fclose(new_file);*/

	// Making directory
	return EXIT_SUCCESS;
}
