#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <dirent.h>
#include <string.h>
#include <errno.h>

#define BUF_SIZE 4096

void copy_file(const char *source, const char *target, int verbose) {
    int source_fd, target_fd;
    ssize_t bytes_read, bytes_written;
    char buffer[BUF_SIZE];

    source_fd = open(source, O_RDONLY);
    if (source_fd == -1) {
        perror("Error opening source file");
        exit(EXIT_FAILURE);
    }

    target_fd = open(target, O_WRONLY | O_CREAT | O_TRUNC, S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH);
    if (target_fd == -1) {
        perror("Error opening target file");
        exit(EXIT_FAILURE);
    }

    while ((bytes_read = read(source_fd, buffer, BUF_SIZE)) > 0) {
        bytes_written = write(target_fd, buffer, bytes_read);
        if (bytes_written != bytes_read) {
            perror("Error writing to target file");
            exit(EXIT_FAILURE);
        }
    }

    close(source_fd);
    close(target_fd);

    if (verbose) {
        printf("Copy File: %s -> %s\n", source, target);
    }
}

void copy_multiple_files(int argc, char *argv[], const char *target, int verbose) {
    if (mkdir(target, 0777) == -1 && errno != EEXIST) {
        perror("Error creating target directory");
        exit(EXIT_FAILURE);
    }

    for (int i = 2; i < argc - 1; i++) {
        char *filename = strrchr(argv[i], '/');
        if (filename == NULL) {
            filename = argv[i];
        } else {
            filename++;
        }
        char target_file[PATH_MAX];
        snprintf(target_file, PATH_MAX, "%s/%s", target, filename);
        copy_file(argv[i], target_file, verbose);
    }
}

void copy_directory(const char *source, const char *target, int verbose) {
    DIR *dir;
    struct dirent *entry;

    dir = opendir(source);
    if (dir == NULL) {
        perror("Error opening source directory");
        exit(EXIT_FAILURE);
    }

    if (mkdir(target, 0777) == -1 && errno != EEXIST) {
        perror("Error creating target directory");
        exit(EXIT_FAILURE);
    }

    while ((entry = readdir(dir)) != NULL) {
        if (strcmp(entry->d_name, ".") != 0 && strcmp(entry->d_name, "..") != 0) {
            char source_path[PATH_MAX];
            char target_path[PATH_MAX];

            sprintf(source_path, "%s/%s", source, entry->d_name);
            sprintf(target_path, "%s/%s", target, entry->d_name);

            copy_file(source_path, target_path, verbose);
        }
    }

    closedir(dir);

    if (verbose) {
        printf("Copied all files and directories from %s to %s\n", source, target);
    }
}

int main(int argc, char *argv[]) {
    int opt;
    int verbose = 0;

    while ((opt = getopt(argc, argv, "fmvd")) != -1) {
        switch (opt) {
            case 'v':
                verbose = 1;
                break;
        }
    }

    optind = 1;
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
        }
    }

    return 0;
}
