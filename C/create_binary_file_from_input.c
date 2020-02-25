/* create temporary/perm binary file from input, for example
input is a string "fface024 4221  2f", the output/file when running using bash command "hexdump -C output/file" will return

00000000 ff ac e0 24 42 21 2f      |...........|
*/


#include <stdio.h> // for fprintf
#include <getopt.h> //get getopt_long()
#include <stdlib.h>
#include <unistd.h>

int main (int argc, char **argv)
{
	// process option 
	int optc;
	const char* filename = NULL;
	int fail = 0;
	static const struct option longopts[] =
	{
		{"file", required_argument, NULL, 'f' },
		{NULL, 0, NULL,0}
	};
	while ((optc = getopt_long (argc, argv, "f:", longopts, NULL)) != -1)
	{
		switch (optc)
		{
			case 'f': 
				filename = optarg;
				break;
			default:
				fail = 1;
				break;
		}
	}
	if ( fail || optind == argc )
	{
		if (optind == argc)
		fprintf( stderr, "%s lack an operand\n", argv[0]);
		fprintf( stderr, "fail\n" );
		return 1;
	}
	if (filename == NULL);
		filename = tmpnam(NULL);
	if (access(filename, F_OK) != -1 &&  access(filename, R_OK | W_OK) == -1)
	{
		fprintf( stderr, "%s file exist but unreadable or unwriteable", filename);
		return 1;
	}

	//merge and erase space in input string, for example "ab cd 01 3" "4ef 20" will be merge into "abcd0134ef20" and will be store in dyn_buffer
	char *dyn_buffer;
	int buffer_length = 0;
	int flag = 0;
	for (int x = optind ;x != argc; x++)
	{
		for (int pointer = 0; argv[x][pointer] != '\x00'; pointer++)
		{
			if (argv[x][pointer] == ' ')
				continue;
			buffer_length++;
		}
	} 
	if (buffer_length % 2 == 1)
	{
		buffer_length += 1;
		flag = 1;
	}
	buffer_length++;
	dyn_buffer = malloc( (buffer_length * sizeof(char))  );
	dyn_buffer[buffer_length-1] = '\x00';
	if (flag == 1 )
		dyn_buffer[buffer_length-2] = '0';
	// put things into this buffer
	// printf ("%d",buffer_length);
	int buffer_pointer = 0 ;
	for (int x = optind ;x != argc; x++)
	{
		for (int pointer = 0; argv[x][pointer] != '\x00'; pointer++)
		{
			if ( argv[x][pointer] == ' ' )
				continue;
			dyn_buffer[buffer_pointer++] = argv[x][pointer]; 
		}
	}
	// open and write to file
	FILE *fp;
	fp = fopen (filename, "wb");
	int number1,number2;
	char tranfer[2];
	tranfer[1] = '\x00';
	// printf ("\n%s\n", dyn_buffer);
	for( int i = 0 ; i < buffer_length-1; i++)
	{
		if( dyn_buffer[i] <= '9' && dyn_buffer[i] >= '0')
			number1 = dyn_buffer[i] - 48;
		if ( dyn_buffer[i] <= 'F' && dyn_buffer[i] >= 'A')
			number1 = dyn_buffer[i] - 55;
		if ( dyn_buffer[i] <= 'f' && dyn_buffer[i] >= 'a')
			number1 = dyn_buffer[i] - 87;	
		i++;
		number1 <<= 4;
		if( dyn_buffer[i] <= '9' && dyn_buffer[i] >= '0')
			number2 = dyn_buffer[i] - 48;
		if ( dyn_buffer[i] <= 'F' && dyn_buffer[i] >= 'A')
			number2 = dyn_buffer[i] - 55;
		if ( dyn_buffer[i] <= 'f' && dyn_buffer[i] >= 'a')
			number2 = dyn_buffer[i] - 87;
		number1 += number2;	
		*tranfer = (char) number1;
		// printf("\n%d\n",number1);
		fwrite(tranfer,1,1,fp);
	}
	fclose(fp);
	free(dyn_buffer);
	printf("%s", filename);
	return 0;
}
		
