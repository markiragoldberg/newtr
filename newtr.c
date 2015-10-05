#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <errno.h>

/*
 * newtr
 * by Mark Ira Goldberg, 2015
 *
 * Mimics the functionality of the UNIX tr command, to a limited extent.
 *
 * Options: -d (delete mode)
 * Interpreted sequences: '\\', '\n', '\r', '\t'
 */

int chpos(char ch, char *set);
//Search set for char ch
//Return index of first instance of ch
//Return -1 if ch is not found in set

void interp_set(char *set);
//Convert interpreted sequences in set to corresponding characters
//Affected sequences are \\, \n, \r, \t

void erroraware_putchar(int character);
//putchar, but with error handling

int main(int argc, char ** argv)
{
	int next;

	//Both usages of newtr should have two arguments + newtr itself
	if(argc != 3)
	{
		printf("Usage: newtr [-d] SET1 [SET2]\n");
		return EXIT_FAILURE;
	}

	//Delete mode
	if(strcmp(argv[1], "-d")==0)
	{
		//Interp_set the one set
		interp_set(argv[2]);
		while((next = getchar()) != EOF)
		{
			if (chpos(next, argv[2]) == -1)
				erroraware_putchar(next);
		}
		if(ferror(stdin) != 0)
			perror("newtr: error reading from stdin");
	}
	//Translate mode
	else
	{
		//Interp both sets
		interp_set(argv[1]);
		interp_set(argv[2]);
		//If if set2 is too long, do nothing
		//If too short, use min logic to prevent overrun
		while((next = getchar()) != EOF)
		{
			int chrep;
			if ((chrep = chpos(next, argv[1])) == -1)
				erroraware_putchar(next);
			else
			{
				//Place last non-terminal character if set2 is shorter than set1
				if(chrep > strlen(argv[2])-1)
					erroraware_putchar(argv[2][strlen(argv[2])-1]);
				else
					erroraware_putchar(argv[2][chrep]);
			}
		}
		if(ferror(stdin) != 0)
			perror("newtr: error reading from stdin");
	}
	return EXIT_SUCCESS;
}

int chpos(const char ch, char *set)
{
	for (int p=0; p<strlen(set); p++)
	{
		if (set[p] == ch)
			return p;
	}

	return -1;
}

void interp_set(char *set)
{
	int in=0;

	while (in < strlen(set))
	{
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
			//Replacing two chars with a special char requires removing
			//the second char in the interpreted sequence, as done here
			if(replace != -1)
			{
				memmove(&set[in], &set[in+1],sizeof(char) * (strlen(set)-in-1));
				set[in-1] = replace;
				set[strlen(set)-1] = '\0';
			}
		}
	}
}

void erroraware_putchar(int character)
{
	if(putchar(character) == EOF)
		perror("newtr: error writing to stdout");
}

