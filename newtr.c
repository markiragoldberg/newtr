#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <errno.h>

int chpos(char ch, char *set);
//Search set for char ch
//Return index of first instance of ch
//Return -1 if ch is not found in set

void interp_set(char *set);
//Convert interpreted sequences in set to corresponding characters
//Affected sequences are \\, \n, \r, \t

int main(int argc, char ** argv)
{

	int next;

	//Debug output
	/*for(int i=0; i<argc; i++)
		printf("argv(%d): %s\n", i, argv[i]);*/

	//Both usages of newtr should have two arguments + newtr itself
	if(argc != 3) {
		printf("Usage: newtr [-d] SET1 [SET2]\n");
		return EXIT_FAILURE; }

	//Delete mode/translate mode?
	if(strcmp(argv[1], "-d")==0) {
		//Interp_set second argument
		interp_set(argv[2]);
		//printf("Delete mode:\n");
		while((next = getchar()) != EOF) {
			if (chpos(next, argv[2]) == -1)
				putchar(next); }}
	else {
		//printf("Translate mode:\n");
		interp_set(argv[1]);
		interp_set(argv[2]);
		//Set2 must match length of set1
		char set2[strlen(argv[1])+1];
		if(strlen(argv[2]) > strlen(argv[1])) {
			//Shorten command line argument so strcpy doesn't segfault
			argv[2][strlen(argv[1])] = '\0'; }
		strcpy(set2, argv[2]);
		if(strlen(argv[2]) < strlen(argv[1])) {
			//Length set2 to match set1
			memset(&set2[strlen(set2)],set2[strlen(set2)-1],sizeof(char)*(strlen(argv[1]) - strlen(argv[2])));
			set2[strlen(argv[1])] = '\0'; }
		printf("set2: %s\n", set2);

		while((next = getchar()) != EOF) {
			int chrep;
			if ((chrep = chpos(next, argv[1])) == -1)
				putchar(next);
			else
				putchar(set2[chrep]);}}

	return EXIT_SUCCESS;
}

int chpos(const char ch, char *set)
{
	for (int p=0; p<strlen(set); p++) {
		if (set[p] == ch)
			return p; }

	return -1;
}

void interp_set(char *set)
{
	int in=0;

	while (in < strlen(set))
	{
		//How 2 handle a '\' that isn't part of a recognized sequence?
		//Or '\' immediately before sentinel \0?
			//Use memmove to shorten string?
			//since you're replacing a two-char sequence with one char
		char next = set[in++];
		if(next == '\\')
		{
			char replace = -1;
			switch (set[in])
			{
			case '\\':
				replace = '\\';
				break;
			case 'n':
				replace = '\n';
				break;
			case 'r':
				replace = '\r';
				break;
			case 't':
				replace = '\t';
				break;
			}

			if(replace != -1)
			{
				memmove(&set[in], &set[in+1],sizeof(char) * (strlen(set)-in-1));
				set[in-1] = replace;
				set[strlen(set)-1] = '\0';
			}
		}
	}
}

