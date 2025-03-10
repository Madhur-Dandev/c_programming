#include <stdio.h>

int main(void)
{
	// scanf, getc, gets, getchar
	/*int student_age[5];
	for(int i = 0; i < (sizeof(student_age)/sizeof(int)); i++)
	{
		printf("Enter age of student %d: ", i+1);
		scanf("%d", &student_age[i]);
	}
	for(int i = 0; i < 5; i++)
	{
		printf("%d\n", student_age[i]);
	}

	char name[5][255];

	for(int i = 0; i < 5; i++)
	{
		printf("Tell your name: ");
		scanf("%s", name[i]);
	}
	
	for(int i = 0; i < 5; i++)
	{
		printf("Student %d: %s", i+1, name[i]);
	}*/

	// gets
	char name[50];
	//fgets(name, 50, stdin);
	int counter = 0;
	while((name[counter++] = fgetc(stdin)) != '\n')
		continue;
	printf("%s", name);
	return 0;
}
