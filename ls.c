//Used for basic input/output stream
#include <stdio.h>
//Used for handling directory files
#include <dirent.h>
//For EXIT codes and error handling
#include <errno.h>
#include <stdlib.h>
//Stats about files and size of files
#include <sys/types.h>
#include <sys/stat.h>
//Functions for working with strings in C
#include <string.h>

void _ls(const char *dir)
{
    
    /* Append output to a log file in the working
       directory. */
    FILE *fp;
    fp = fopen("/home/max/Documents/Project/log.txt", "w+");
    char fullpath[PATH_MAX+1];
    
    /* Structure for using dirent.h functions */
    struct dirent *d;
    
    /* Dir object for navigation */
    DIR *dh = opendir(dir);
    
    /* Counter for better navigating through file */
    int counter = 1;

    /* Structure for stats about file like size and other */
    struct stat sb;

    if(!dh)
    {
        if(errno = ENOENT)
        {
            // directory not found
            perror("Directory doesn't exist\n");
        }
        else
        {
            // if the directory is not readable then throw error
            perror("Unable to read directory\n");
        }
        exit(EXIT_FAILURE);
    }

    /* until the next entry is not readable we will print files */
    while((d = readdir(dh)) != NULL)
    {

        /* string for fullpath, made by appending the constant
           directory with the name of the current file. */
        char filepath[PATH_MAX+1];
        strcpy(filepath, dir);
        strcat(filepath, "/");
        strcat(filepath, d->d_name);
        realpath(filepath, fullpath);

        /* test for errors */
        if(stat(fullpath, &sb) == -1)
        {
            perror("something went wrong!\n");
            EXIT_FAILURE;
        }
        

        /* test if file is dir, if yes then return non-zero value and print DIR: */
        if(S_ISDIR(sb.st_mode) != 0)
        {
            if(strcmp(d->d_name, ".") == 0 || strcmp(d->d_name, "..") == 0)
            {
                if(counter < 10)
                {
                    fprintf(fp, "DIR:  %d. [%s/%s] = %lld bytes\n", counter, fullpath, d->d_name, (long long)sb.st_size);
                }
                else
                {
                    fprintf(fp, "DIR:  %d.[%s/%s] = %lld bytes\n", counter, fullpath, d->d_name, (long long)sb.st_size);
                }
                counter++;
            }
        }

        /* test if file is regular file, if yes then return non-zero value and print FILE: */
        if(S_ISREG(sb.st_mode) != 0)
        {

            /* just so that it is alligned, so it's better looking */
            if(counter < 10)
            {
                fprintf(fp, "FILE: %d. [%s] = %lld bytes\n", counter, fullpath, (long long)sb.st_size);
            }
            else
            {
                fprintf(fp, "FILE: %d.[%s] = %lld bytes\n", counter, fullpath, (long long)sb.st_size);
            }
            counter++;
        }
    }


    /* close file for no errors */
    fclose(fp);
}

int main(int argc, const char *argv[])
{
    char path[PATH_MAX+1];

    printf("ENTER FULLPATH TO YOUR DIRECTORY: ");
    scanf("%s", (char *)&path);
    printf("\n\n\n\t\t    -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.\n\t\t-.-.Your output is in file named <log.txt>-.-.\n\t\t    -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.\n");
    _ls(path);
    return 0;
}