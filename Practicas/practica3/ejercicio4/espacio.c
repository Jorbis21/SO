#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <dirent.h>
#include <string.h>

int get_size_dir(char *dname, size_t *blocks);

/* Gets in the blocks buffer the size of file fname using lstat. If fname is a
 * directory get_size_dir is called to add the size of its contents.
 */
int get_size(char *fname, size_t *blocks){
	struct stat *fileInfo = malloc(sizeof(struct stat));
	lstat(fname, fileInfo);
	if(S_ISDIR(fileInfo->st_mode)){
		get_size_dir(fname, blocks);
	}
	*blocks += fileInfo->st_blocks;
	return EXIT_SUCCESS;
}


/* Gets the total number of blocks occupied by all the files in a directory. If
 * a contained file is a directory a recursive call to get_size_dir is
 * performed. Entries . and .. are conveniently ignored.
 */
int get_size_dir(char *dname, size_t *blocks){
	DIR *dir;
	struct stat *dInfo = malloc(sizeof(struct stat));
	struct dirent *dirInfo = malloc(sizeof(struct dirent));
	lstat(dname, dInfo);
	if((dir = opendir(dname)) == NULL){
		printf("Can not open de directory\n");
		return EXIT_FAILURE;
	}
	while ((dirInfo = readdir(dir)) != NULL){
		if (strcmp(dirInfo->d_name,".") & strcmp(dirInfo->d_name,"..")){
			get_size(dirInfo->d_name, blocks);
		}
	}
	*blocks += dInfo->st_blocks;
	closedir(dir);
	return EXIT_SUCCESS;
}

/* Processes all the files in the command line calling get_size on them to
 * obtain the number of 512 B blocks they occupy and prints the total size in
 * kilobytes on the standard output
 */
int main(int argc, char *argv[]){
	int i = 1;
	size_t tam = 0;
	while(i < argc){
		get_size(argv[i], &tam);
		printf("%i%s %s\n", tam/2, "K",argv[i]);
		++i; tam = 0;
	}
	return 0;
}
