#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <dirent.h>
#include <string.h>

#define BUF_SIZE 4096

void copy_file(const char *source, const char *target, int verbose) {
    int source_fd, target_fd;
    ssize_t bytes_read, bytes_written;
    char buffer[BUF_SIZE];

    // Open source file
    source_fd = open(source, O_RDONLY);
    if (source_fd == -1) {
        perror("Error opening source file");
        exit(EXIT_FAILURE);
    }

    // Open target file
    target_fd = open(target, O_WRONLY | O_CREAT | O_TRUNC, S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH);
    if (target_fd == -1) {
        perror("Error opening target file");
        exit(EXIT_FAILURE);
    }

    // Copy file contents
    while ((bytes_read = read(source_fd, buffer, BUF_SIZE)) > 0) {
        bytes_written = write(target_fd, buffer, bytes_read);
        if (bytes_written != bytes_read) {
            perror("Error writing to target file");
            exit(EXIT_FAILURE);
        }
    }

    // Close files
    close(source_fd);
    close(target_fd);

    if (verbose) {
        printf("Copied %s to %s\n", source, target);
    }
}

void copy_multiple_files(int argc, char *argv[], const char *target, int verbose) {
    for (int i = 2; i < argc - 1; i++) {
        copy_file(argv[i], target, verbose);
    }
}

void copy_directory(const char *source, const char *target, int verbose) {
    DIR *dir;
    struct dirent *entry;

    // Open source directory
    dir = opendir(source);
    if (dir == NULL) {
        perror("Error opening source directory");
        exit(EXIT_FAILURE);
    }

    // Create target directory
    if (mkdir(target, 0777) == -1) {
        perror("Error creating target directory");
        exit(EXIT_FAILURE);
    }

    // Copy files from source directory to target directory
    while ((entry = readdir(dir)) != NULL) {
        if (strcmp(entry->d_name, ".") != 0 && strcmp(entry->d_name, "..") != 0) {
            char source_path[PATH_MAX];
            char target_path[PATH_MAX];

            sprintf(source_path, "%s/%s", source, entry->d_name);
            sprintf(target_path, "%s/%s", target, entry->d_name);

            copy_file(source_path, target_path, verbose);
        }
    }

    // Close directory
    closedir(dir);

    if (verbose) {
        printf("Copied all files and directories from %s to %s\n", source, target);
    }
}

int main(int argc, char *argv[]) {
    int opt;
    int verbose = 0;

    // Parse options
    while ((opt = getopt(argc, argv, "fmvd")) != -1) {
        switch (opt) {
            case 'f':
                copy_file(argv[2], argv[3], verbose);
                break;
            case 'm':
                copy_multiple_files(argc, argv, argv[argc - 1], verbose);
                break;
            case 'd':
                copy_directory(argv[2], argv[3], verbose);
                break;
            case 'v':
                verbose = 1;
                break;
            default:
                fprintf(stderr, "Usage: %s -[f|m|d] [-v] Source Target\n", argv[0]);
                exit(EXIT_FAILURE);
        }
    }

    return 0;
}
