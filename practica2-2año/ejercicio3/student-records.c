#include <stdio.h>
#include <unistd.h> /* for getopt() */
#include <stdlib.h> /* for EXIT_SUCCESS, EXIT_FAILURE */
#include <string.h>
#include <getopt.h>
#include "defs.h"


/* Assume lines in the text file are no larger than 100 chars */
#define MAXLEN_LINE_FILE 100

int print_text_file(char *path)
{
	/* To be completed (part A) */
	FILE *studentF;
	student_t student;
	char *aux = malloc(sizeof(char)), *buffer = malloc(sizeof(student_t));
	short int numStudent, i = 0;
	studentF = fopen(path, "r");
	if(studentF == NULL){
		printf("Failed to open the file %s", path);
	}
	/**Hacer esta parte con fgets para meterlo todo en un buffer to tocho
	 * usar el strsep para coger los datos y mostrarlos
	*/
	fread(aux, sizeof(char), 1 ,studentF);
	numStudent = atoi(aux);
	while(feof(path) != 0 && i < numStudent){
		fgets(buffer, sizeof(student_t), studentF);
		strsep(buffer, ":");
		student.student_id = atoi();//revisar bien como funciona strsep
		*student.NIF = strsep(buffer, ":");
		student.first_name = strsep(buffer, ":");
		student.last_name = strsep(buffer, ":");
		printf("[ENTRY #%d]\n", i);
		printf("student_id=%d", student.student_id);
		printf("NIF=%s", student.NIF);
		printf("first_name=%s", student.first_name);
		printf("last_name=%s", student.last_name);
		i++;
	}
	return 0;
}

int print_binary_file(char *path)
{
	/* To be completed  (part B) */
	return 0;
}


int write_binary_file(char *input_file, char *output_file)
{
	/* To be completed  (part C) */
	return 0;
}

int main(int argc, char *argv[])
{
	int ret_code, opt;
	struct options options;

	/* Initialize default values for options */
	options.input_file = NULL;
	options.output_file = NULL;
	options.action = NONE_ACT;
	ret_code = 0;

	/* Parse command-line options (incomplete code!) */
	while ((opt = getopt(argc, argv, "hi:p")) != -1)
	{
		switch (opt)
		{
		case 'h':
			fprintf(stderr, "Usage: %s [ -h ]\n", argv[0]);
			exit(EXIT_SUCCESS);
		case 'i':
			options.input_file = optarg;
			break;
		case 'p':
			options.action = PRINT_TEXT_ACT;
			break;
		/**
		 **  To be completed ...
		 **/

		default:
			exit(EXIT_FAILURE);
		}
	}

	if (options.input_file == NULL)
	{
		fprintf(stderr, "Must specify one record file as an argument of -i\n");
		exit(EXIT_FAILURE);
	}

	switch (options.action)
	{
	case NONE_ACT:
		fprintf(stderr, "Must indicate one of the following options: -p, -o, -b \n");
		ret_code = EXIT_FAILURE;
		break;
	case PRINT_TEXT_ACT:
		/* Part A */
		ret_code = print_text_file(options.input_file);
		break;
	case WRITE_BINARY_ACT:
		/* Part B */
		ret_code = write_binary_file(options.input_file, options.output_file);
		break;
	case PRINT_BINARY_ACT:
		/* Part C */
		ret_code = print_binary_file(options.input_file);
		break;
	default:
		break;
	}
	exit(ret_code);
}
