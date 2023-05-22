//Anthony White
//CSE 130 Assignment 0
//Split: Splits any number of input files with a newline at a given delimiter

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <ctype.h>
#include <string.h>
#include <err.h>
#include <errno.h>
#include <stdbool.h>

#define MAXBUFFLEN 4096

void handleFile(int, char);

int main(int argc, char **argv) {
    char multiCharError[] = "Cannot handle multi-character splits: ";
    char fewArgsError[] = "Not enough arguments\n";
    char usageError[] = "usage: ./split: <split_char> [<file1> <file2> ...]\n";
    int fileCtr = 1;
    char delimiter;
    if (argc > 2 && argv[1] != NULL) {
        if (strlen(argv[1]) == 1) {
            delimiter = *argv[1];
            fileCtr++;
        } else {
            write(2, multiCharError, strlen(multiCharError));
            write(2, argv[1], strlen(argv[1]));
            write(2, "\n", 1);
            write(1, usageError, strlen(usageError));
            //errno =
            exit(EINVAL);
        }
    } else {
        write(2, fewArgsError, strlen(fewArgsError));
        write(1, usageError, strlen(usageError));
        exit(EINVAL);
    }
    while (fileCtr < argc) {
        int fp = 0;
        if (strcmp(argv[fileCtr], "-") == 0) {
            fp = STDIN_FILENO;
            handleFile(fp, delimiter);
            close(fp);
        } else {
            int flags = 0;
            if (access(argv[fileCtr], R_OK) == 0) {
                //printf("%s", "test\n");
                fp = open(argv[fileCtr], flags, O_RDONLY);
                handleFile(fp, delimiter);
                close(fp);
            } else {
                warn("%s", argv[fileCtr]);
            }
        }
        fileCtr++;
    }
    return errno;
}

void handleFile(int fp, char delimiter) {
    char preBuffer[MAXBUFFLEN];
    ssize_t readSize = 1; //To check when eof occurs
    while (readSize > 0) {
        readSize = read(fp, preBuffer, MAXBUFFLEN);
        if (readSize == -1) {
            warn(NULL);
            return;
        }
        for (int i = 0; i < readSize; i++) {
            if (preBuffer[i] == delimiter) {
                preBuffer[i] = '\n';
            }
        }
        ssize_t writeGood = write(1, preBuffer, readSize);
        if (writeGood == -1) {
            warn(NULL);
            return;
        }
    }
}
