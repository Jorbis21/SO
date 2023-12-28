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
	char *aux = malloc(sizeof(char)), *buffer;
	short int numStudent, i = 0;
	studentF = fopen(path, "r");
	if(studentF == NULL){
		printf("Failed to open the file %s", path);
	}
	
	fgets(aux, sizeof(char) + 1, studentF);
	numStudent = atoi(aux);
	fgets(aux, sizeof(char) + 1, studentF);
	while(i < numStudent){
		buffer = malloc(sizeof(student_t));
		fgets(buffer, sizeof(student_t), studentF);
		student.student_id = atoi(strsep(&buffer, ":"));//revisar bien como funciona strsep
		strcpy(student.NIF, strsep(&buffer, ":"));
		student.first_name = strsep(&buffer, ":");
		student.last_name = strsep(&buffer, ":");
		student.last_name[strlen(student.last_name)-1] = '\0';

		printf("[ENTRY #%d]\n", i);
		printf("	student_id=%d\n", student.student_id);
		printf("	NIF=%s\n", student.NIF);
		printf("	first_name=%s\n", student.first_name);
		printf("	last_name=%s\n", student.last_name);
		i++;
		free(buffer);
	}
	fclose(studentF);
	return 0;
}

int print_binary_file(char *path)
{
	/* To be completed  (part B) */
	return 0;
}


int write_binary_file(char *input_file, char *output_file)
{
	if (strstr(output_file, ".bin") == NULL)
	{
		fprintf(stderr, "Formato de archivo no admitido. Use '.bin'\n");
		exit(EXIT_FAILURE);
	}
	FILE *studentF, *studentFB;
	student_t student;

	char *aux = malloc(sizeof(char)), *buffer;
	short int numStudent, i = 0;

	studentF = fopen(input_file, "r");
	studentFB = fopen(output_file, "w");

	if(studentF == NULL || studentFB == NULL){
		printf("Failed to open the file %s or %s", input_file, output_file);
	}

	fgets(aux, sizeof(char)+1, studentF);
	numStudent = atoi(aux);
	fgets(aux, sizeof(char)+1, studentF);
	sprintf(aux, "%d", numStudent);
	fwrite(aux, sizeof(int), 1, studentFB);
	while(i < numStudent){
		//Lectura de los datos
		buffer = malloc(sizeof(student_t));
		fgets(buffer, sizeof(student_t), studentF);
		student.student_id = atoi(strsep(&buffer, ":"));
		strcpy(student.NIF, strsep(&buffer, ":"));
		student.first_name = strsep(&buffer, ":");
		student.last_name = strsep(&buffer, ":");
		student.last_name[strlen(student.last_name)-1] = '\0';
		//Escritura de los datos
		sprintf(aux, "%d", student.student_id);
		fwrite(aux, sizeof(int), 1, studentFB);
		fwrite(student.NIF, sizeof(student.NIF), 1, studentFB);
		fwrite(student.first_name, sizeof(student.first_name), 1, studentFB);
		fwrite(student.last_name, sizeof(student.last_name), 1, studentFB);
		free(buffer);
		i++;
	}
	fclose(studentF);
	fclose(studentFB);
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
	while ((opt = getopt(argc, argv, "hi:po:")) != -1)
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
		case 'o':
			options.action = WRITE_BINARY_ACT;
			options.output_file = optarg;
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
