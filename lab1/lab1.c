/*+
 * lab1.c - Print process information
-*/

#include <stdio.h>
#include <stdlib.h>
#include <dirent.h>
#include <string.h>
#include <strings.h>
#include <ctype.h>

int isProcessDir(const struct dirent*d) {
    char *tmp = d->d_name;

    // Iterate until the end of the string
    while (*tmp != '\0') {
        // Check if the character is a digit
        if (isdigit(*tmp) == 0)
            return 0;
        tmp++;
    }
    return 1;
}

void printDirectories(struct dirent **directoryList, int n) {
    int i;
    for (i = 0; i < n; i++) {
        printf("%s \n", directoryList[i]->d_name);
    }
    printf("\n");
}

void splitString(char *string, char *result, int n) {
    int i;
    int idx = 0;
    for (i = n; i < strlen(string); i++) {
        if (string[i] != ' ') {
            result[idx] = string[i];
            idx++;
        }
    }
    result[strlen(result)] = '\0';
}

int main() {
    struct dirent ** namelist;
    int numDirectories; // Stores the number of directories read by scandir


    numDirectories = scandir("/proc", &namelist, isProcessDir, NULL);

    // Check if no directories were read
    if (numDirectories == -1) {
        printf("The /proc directory could not be read\n");
        exit(0);
    }

    int i;
    for (i = 0; i < numDirectories; i++) {
        char path[100];
        char buffer[100];
        char processInfo[4][100];

        sprintf(path, "/proc/%s/status", namelist[i]->d_name);
        FILE *process = fopen(path, "r");

        // Check if the file could not be opened
        if (process == NULL) {
            printf("The process could not be read\n");
            exit(0);
        }

        while (fgets(buffer, 100, process)) {
            if (strncmp(buffer, "Name", 4) == 0) {
                buffer[strlen(buffer) - 1] = '\0';
                // char result[100];
                // splitString(buffer, result, 4);
                strcpy(processInfo[0], buffer + 6);
            } else if (strncmp(buffer, "State", 5) == 0) {
                buffer[strlen(buffer) - 1] = '\0';
                strcpy(processInfo[1], buffer);
            } else if (strncmp(buffer, "Uid", 3) == 0) {
                buffer[strlen(buffer) - 1] = '\0';
                strcpy(processInfo[2], buffer);
            } else if (strncmp(buffer, "Gid", 3) == 0) {
                buffer[strlen(buffer) - 1] = '\0';
                strcpy(processInfo[3], buffer);
            }
        }

        printf("%s\n", processInfo[0]);
        printf("%s\n", processInfo[1]);
        printf("%s\n", processInfo[2]);
        printf("%s\n", processInfo[3]);
        printf("\n");
        fclose(process);
    }
    // printDirectories(namelist, numDirectories);
    return 0;
}
