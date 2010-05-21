#include "utils.h"

#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>
#include <sys/stat.h>

int
p_strsplit(char *buf, char **seg)
{
	char *x;
	int i = 0;
	seg = NULL;

	x = strtok (buf, "\n");
	while (x != NULL) {
		i++;
		seg = (char **)realloc ((char *)seg, sizeof (char *));
		seg[i-1] = x;
		x = strtok (NULL, "\n");
	}

	return i;
}

char *
p_strcpy(char *str)
{
        //allocates and copies a string
	char *new = malloc (sizeof (char) * strlen (str));
	strcpy (new, str);
	return new;
}

int
search_match(char **tree, char *apple)
{
	int i = 0;
	while (strcmp (apple, tree[i]) != 0) {
		i += 2;
		if (strcmp ("\0", tree[i]) == 0)
			return -1;
	}
	return i;
}

double
getTime()
{
        //check and return time
	struct timeval curTime;
	(void) gettimeofday (&curTime, (struct timezone *) NULL);
	return (((((double) curTime.tv_sec) * 1000000.0) 
             + (double) curTime.tv_usec) / 1000000.0); 
}

void
die_with_error(char *error_message)
{
    perror(error_message);
    exit(1);
}

int fsize(const char *f) {
	struct stat st; 

	if (stat(f, &st) == 0)
                //return file size
		return st.st_size;

        //return error
	return -1; 
}

int
fcheck_for_user(const char *user)
{
	struct stat st;
	char *path = "/home/";
	char *folder = NULL;
        //build location of home directory
	folder = (char *)malloc(strlen(path)+strlen(user)+1);
	memset(folder,0,strlen(path)+strlen(user)+1);

	strcat(folder,path);
	strcat(folder,user);
	strcat(folder,"/");
        //check if present
	if(stat(folder,&st) == 0) {
		printf(" %s is present\n", folder);
		free(folder);
		return 0;
	}

	free(folder);
        //return error
	return -1; 
}

int
fcheck_for_file(const char *file)
{
	struct stat st;
        //check if file exists
	if(stat(file,&st) == 0) {
		printf(" %s is present\n", file);
		return 0;
	}

	return -1; 
}

//Take path of file EX: '/this/is/a/test.txt' an return 'test.txt'
char *
fileFromPath(char * path) {
    int len = strlen(path);
    int j = 0;
    int i = len;

    while(i != 0 && path[i] != '/'){
        j++;
        i--;
    }
    if(path[i] == '/') {
        j--;
    }
    char * result = malloc(j);
    strncpy(result, path + (len - j), j);

    return result;
}
