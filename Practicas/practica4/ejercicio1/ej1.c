#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <err.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/wait.h>
#include <sys/signal.h>
#include <dirent.h>
#include <errno.h>
#include <getopt.h>
#include <limits.h>
#include <string.h>
struct options {
	char *progname;
	int recurse;
};

struct options opt;

void usage(void)
{
	printf("%s [options] [dirname]\n\n", opt.progname);
	printf("lists the contents of dirname (default .)\n");
	printf("options:\n"
		"\t-h:\tshow this help\n"
		"\t-R:\trecursive\n"
	);
}

/* apartado b */
void list_dir(char *name){
	DIR *d;
	struct dirent *de = malloc(sizeof(struct dirent));
	if((d = opendir(name)) == NULL){
		printf("Can not open the directory\n");
		exit(EXIT_FAILURE);
	}
	while ((de = readdir(d)) != NULL){
		printf("%s\n", de->d_name);
	}
	printf("\n");
	closedir(d);
}

/* apartado c */
void process_recurse(char *dirname, char *name)
{
	pid_t pid;
	char path[PATH_MAX];
	int status;

	pid = fork();
	if(pid == 0){
		//aqui falta un wait para el resto de procesos
		strcpy(path, dirname);
		strcat(path, "/");
		strcat(path, name);
		execlp("./ej1", "./ej1", "-R",path, NULL);
		exit(EXIT_SUCCESS);
	}
	else{
		waitpid(pid, &status,0);
	}
}

/* apartado c */
void list_dir_recurse(char *name)
{
	DIR *d;
	struct dirent *de = malloc(sizeof(struct dirent));
	struct stat *fileInfo;

	if((d = opendir(name)) == NULL){
		exit(EXIT_FAILURE);
	}
	while ((de = readdir(d)) != NULL){
		fileInfo = malloc(sizeof(struct stat));
		stat(de->d_name,fileInfo);
		if(S_ISDIR(fileInfo->st_mode)){
			if(strcmp(de->d_name, ".") & strcmp(de->d_name, "..")){
				process_recurse(name, de->d_name);
			}
		}
	}
	closedir(d);
}

int main(int argc, char **argv)
{
	char *dirname = ".";
	int o;
	opt.progname = argv[0];
	opt.recurse = 0;

	while((o = getopt(argc, argv, "hR")) != -1){
		switch (o){
		case 'h':
			usage();
			exit(EXIT_SUCCESS);
		case 'R':
			opt.recurse = 1;
			break;
		}
	}
	if(optind == (argc-1)){
		dirname = argv[optind];
	}
	if (opt.recurse)
		printf("%s:\n", dirname);

	list_dir(dirname);

	if (opt.recurse)
		list_dir_recurse(dirname);

	return EXIT_SUCCESS;
}
