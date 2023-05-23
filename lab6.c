#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <dirent.h>
#include <string.h>
#include <stdbool.h>

#define BUF_SIZE 4096
#define MAX_LENGTH 4096

bool isDirectory(char* path)
{
	struct stat ss;
	stat(path, &ss);
	return S_ISDIR(ss.st_mode) ? true : false;
}

void file_to_file(const char* src_path, const char* dest_path) {
    int src_fd, dest_fd, n;
    char buf[BUF_SIZE];

    src_fd = open(src_path, O_RDONLY);
    if (src_fd == -1) {
        printf("OpenError\n");
        return;
    }

    if((dest_fd = open(dest_path, O_CREAT | O_RDWR, S_IRWXU | S_IRWXG | S_IRWXO)) == -1) {
		printf("OpenError\n");
		return;
	}

    while ((n = read(src_fd, buf, BUF_SIZE)) > 0) {
        if (write(dest_fd, buf, n) != n) {
            printf("WriteError\n");
            close(src_fd);
            close(dest_fd);
            return;
        }
    }

    close(src_fd);
    close(dest_fd);
}

void dir_to_dir(const char* src_dir, const char* dest_dir) {
    DIR* dir;
    struct dirent* src;
    char src_path[MAX_LENGTH];
    char dest_path[MAX_LENGTH];

	dir = opendir(src_dir);
    if (dir == NULL) {
        printf("No Such a dir\n");
        return;
    }

    if(opendir(dest_dir) == NULL) {
    	if (mkdir(dest_dir, S_IRWXU | S_IRWXG | S_IRWXO) == -1) {
        	printf("Error when making a new dir\n");
        	return;
    	}
    }

    while ((src = readdir(dir))!= NULL) {
        if (strcmp(src->d_name, ".") == 0 || strcmp(src->d_name, "..") == 0)
            continue;

        snprintf(src_path, MAX_LENGTH, "%s/%s", src_dir, src->d_name);
        snprintf(dest_path, MAX_LENGTH, "%s/%s", dest_dir, src->d_name);

        if (src->d_type == DT_DIR) {
            dir_to_dir(src_path, dest_path);
        }
        else if (src->d_type == DT_REG) {
            file_to_file(src_path, dest_path);
        }
    }
    closedir(dir);
}

int main(int argc, char* argv[]) {
    if (argc != 3) {
        printf("Arguments Error\n");
        return 1;
    }
	char *src=argv[1];
	char *dest=argv[2];
	if(isDirectory(src))
		{
			dir_to_dir(src,dest);
		}else{
			file_to_file(src,dest);
		}
 
    return 0;
}
