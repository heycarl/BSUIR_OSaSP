#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include <limits.h>

#include <sys/stat.h>
#include <dirent.h>
#include <unistd.h>
#include <stdbool.h>

#define FLAG_SYMLINK      0
#define FLAG_DIRECTORIES  1
#define FLAG_FILES        2

#define PRINT_DIR printf("\033[1;32m");   // Green
#define PRINT_FILE printf("\033[0;37m");  // Blue
#define PRINT_LINK printf("\033[0;36m");  // Cyan
#define PRINT_OFF printf("\033[0m");      // Reset

void directory_browsing( char *introducedDir,  bool* flags ) // NOLINT(misc-no-recursion)
{
    DIR *dir = NULL;
    struct dirent *entry = NULL;
    char pathName[PATH_MAX + 1];
    dir = opendir(introducedDir);
    if( dir == NULL ) {
    	printf( "Error opening %s: %s", introducedDir, strerror(errno));
    	return;
    }
	entry = readdir(dir);
	while(entry != NULL) {
    	if((strncmp(entry->d_name, ".", PATH_MAX) == 0) || (strncmp(entry->d_name, "..", PATH_MAX) == 0)) {
        	entry = readdir(dir);
        	continue;
    	}
	    strncpy(pathName, introducedDir, PATH_MAX);
        strncat(pathName, "/", PATH_MAX);
        strncat(pathName, entry->d_name, PATH_MAX);

        struct stat entryInfo;
        if(lstat(pathName, &entryInfo) == 0) {
            if(S_ISDIR(entryInfo.st_mode)) {
			    if(flags[FLAG_DIRECTORIES]) {
                    PRINT_DIR printf("%s\n", pathName); PRINT_OFF
			    }
                directory_browsing(pathName, flags);
            } else if(S_ISREG(entryInfo.st_mode) && (flags[FLAG_FILES])) {
//            		PRINT_FILE printf("%s has %lld bytes\n", pathName, (long long)entryInfo.st_size); PRINT_OFF
            		PRINT_FILE printf("%s \n", pathName); PRINT_OFF
            } else if(S_ISLNK(entryInfo.st_mode)) {
                char targetName[PATH_MAX + 1];
                if(readlink(pathName, targetName, PATH_MAX) != -1) {
				    if(flags[FLAG_SYMLINK])
            			PRINT_LINK printf("%s -> %s\n", pathName, targetName); PRINT_OFF
                } else {
				if(flags[FLAG_SYMLINK])
            				printf("%s -> (invalid symbolic link!)\n", pathName);
                }
            }
        } else {
            printf("Error starting %s: %s\n", pathName, strerror(errno));
        }
 	    entry = readdir(dir);
	}
	closedir(dir);
}

bool* parse_options (int argc, char** args) {
    bool* arg_flags = (bool*) calloc(3, sizeof (bool));
    if (argc < 2)
        return arg_flags;
    int flag_search_result;
    while ( (flag_search_result = getopt(argc, args, "dfls?")) != -1){
        switch (flag_search_result) {
            case 'd': arg_flags[FLAG_DIRECTORIES] = 1; break;
            case 'f': arg_flags[FLAG_FILES] = 1; break;
            case 'l': arg_flags[FLAG_SYMLINK] = 1; break;
            case '?':
                printf("Usage: -d -- directories, -f -- files, -l -- links\n");
                free(arg_flags);
                exit(EXIT_SUCCESS);
            default: continue;
        }
    }
    return arg_flags;
}

int main(int argc, char **argv)
{
	char option[PATH_MAX];
	char direct[PATH_MAX];
    bool* flags = parse_options(argc, argv);
    switch (argc) {
        case 1: { // no args
            strcpy(direct,".");
            flags[FLAG_SYMLINK] = 1;
            flags[FLAG_FILES] = 1;
            flags[FLAG_DIRECTORIES] = 1;
            break;
        }
        case 2: { // only flags
            if(flags[FLAG_FILES] || flags[FLAG_DIRECTORIES]|| flags[FLAG_SYMLINK]) {
                strncpy(option, argv[1], PATH_MAX);
                strcpy(direct,".");
            }
            break;
        }
        case 3: { // flags and dir
            strncpy(direct, argv[2], PATH_MAX);
            break;
        }
        default:
            return 1;
    }
    directory_browsing(direct, flags );
    free(flags);
    return 0;
}
