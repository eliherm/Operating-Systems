/*+
 * lab1.c - Print process information
-*/

#include <stdio.h>
#include <stdlib.h>
#include <dirent.h>
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
        char path[100];  // Buffer to hold the path
        char pName[100]; //Buffer to hold the process name
        char buffer[4][100]; //Buffer to hold the process name
        sprintf(path, "/proc/%s/status", namelist[i]->d_name);

        FILE *process = fopen(path, "r");

        // Check if the file could not be opened
        if (process == NULL) {
            printf("The process could not be read\n");
            exit(0);
        }

        fgets(buffer[0], 100, process);
        fgets(buffer[1], 100, process);
        process++;
        fgets(buffer[2], 100, process);

        printf("%s\n", buffer[0]);
        printf("%s\n", buffer[1]);
        printf("%s\n", buffer[2]);
        puts("\n");
    }

    // printDirectories(namelist, numDirectories);
    return 0;
}
