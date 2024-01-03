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

		printf("[ENTRY #%d]\n", i);
		printf("	student_id=%s\n", strsep(&buffer, ":"));
		printf("	NIF=%s\n", strsep(&buffer, ":"));
		printf("	first_name=%s\n", strsep(&buffer, ":"));
		printf("	last_name=%s", strsep(&buffer, ":"));
		i++;
		free(buffer);
	}
	fclose(studentF);
	return 0;
}

int print_binary_file(char *path)
{
	FILE *studentFB;
	student_t student;
	char *aux = malloc(sizeof(char)), *buffer;
	short int numStudent, i = 0;
	studentFB = fopen(path, "rb");

	if(studentFB == NULL){
		printf("Failed to open the file %s", path);
	}
	
	fread(aux, sizeof(int), 1, studentFB);
	numStudent = atoi(aux);
	while(i < numStudent){
		buffer = malloc(sizeof(student_t));
		printf("[ENTRY #%d]\n", i);
		fread(buffer, sizeof(int), 1, studentFB);
		printf("	student_id=%s\n", buffer);
		fread(buffer, MAX_CHARS_NIF+1, 1, studentFB);
		printf("	NIF=%s\n", buffer);
		buffer[MAX_CHARS_NIF-1] = '\0';
		fread(buffer, sizeof(student.first_name), 1, studentFB);
		printf("	first_name=%s\n", buffer);
		fread(buffer, sizeof(student.last_name), 1, studentFB);
		printf("	last_name=%s\n", buffer);
		i++;
		free(buffer);
	}
	fclose(studentFB);
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
	studentFB = fopen(output_file, "wb");

	if(studentF == NULL || studentFB == NULL){
		printf("Failed to open the file %s or %s", input_file, output_file);
	}

	fgets(aux, sizeof(char)+1, studentF);
	numStudent = atoi(aux);
	fwrite(aux, sizeof(int), 1, studentFB);
	fgets(aux, sizeof(char)+1, studentF);
	while(i < numStudent){
		//Lectura de los datos
		buffer = malloc(sizeof(student_t));
		fgets(buffer, sizeof(student_t), studentF);
		buffer[strlen(buffer)-1] = '\0';
		student.student_id = atoi(strsep(&buffer, ":"));
		strcpy(student.NIF, strsep(&buffer, ":"));
		student.first_name = strsep(&buffer, ":");
		student.last_name = strsep(&buffer, ":");
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
	while ((opt = getopt(argc, argv, "hi:po:b")) != -1)
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
		case 'b':
			options.action = PRINT_BINARY_ACT;
			break;
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
