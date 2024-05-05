#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>


int main(int argc, char *argv[]) {
    /*-----------------------------------check arguments-----------------------------------*/
    if (argc < 4) {
        fprintf(stderr, "Error! Use: %s <OUTPUT> <ERROR> <PROGRAM> [<PARAM>. . .]\n", argv[0]);
        exit(EXIT_FAILURE);
    }

    /*-----------------------------------handle output-----------------------------------*/
    if (strcmp(argv[1], "-") != 0) {
        int output_dir = creat(argv[1], 00700); //open(argv[1], O_WRONLY | O_CREAT | O_TRUNC)

        if (output_dir == -1) {
            perror("Failed to open the output file\n");
            exit(EXIT_FAILURE);
        }
        if (dup2(output_dir, STDOUT_FILENO) == -1) {
            perror("Failed to redirect the standard output\n");
            exit(EXIT_FAILURE);
        }

        close(output_dir);
    }

    /*-----------------------------------handle error-----------------------------------*/
    if (strcmp(argv[2], "-") != 0) {
        int error_dir = creat(argv[2], 00700);

        if (error_dir == -1) {
            perror("Failed to open error file");
            exit(EXIT_FAILURE);
        }
        if (dup2(error_dir, STDERR_FILENO) == -1) {
            perror("Failed to redirect standard error\n");
            exit(EXIT_FAILURE);
        }

        close(error_dir);
    }

    execvp(argv[3], &argv[3]);
    perror("Failed to execute program\n");
    return EXIT_FAILURE;
}
