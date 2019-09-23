/*+
 * lab1.c - Print process information
-*/

#include <stdio.h>
#include <stdlib.h>
#include <dirent.h>
#include <string.h>
#include <strings.h>
#include <ctype.h>

#define BUFFER_LEN 100

// Checks if a directory is a process
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

// Extract the first number from Gid and Pid
void extractFirst (char str[], char result[]) {
    char *ptr = NULL;
    ptr = strtok(str, "\t");
    if (ptr == NULL) return;

    int i;
    for (i = 0; i < 2; i++) {
        ptr = strtok(NULL, "\t");
    }
    strcpy(result, ptr);
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

    // Print the headings
    printf("%-15s\t%-15s\t%-15s\t%-15s\t%-15s\n", "PID", "Name", "Status", "User", "Group");
    printf("------------------------------------------------------------------------\n");

    // Iterates through the directories in /proc
    int i;
    for (i = 0; i < numDirectories; i++) {
        char path[BUFFER_LEN];
        char buffer[BUFFER_LEN];
        char processInfo[5][BUFFER_LEN];

        sprintf(path, "/proc/%s/status", namelist[i]->d_name);
        FILE *process = fopen(path, "r");

        // Check if the file could not be opened
        if (process == NULL) {
            printf("The process could not be read\n");
            exit(0);
        }
        strcpy(processInfo[0], namelist[i]->d_name);

        // Iterate through all lines of the file
        // Look for specific rows (Name, State, Uid, Gid)
        while (fgets(buffer, BUFFER_LEN, process)) {
            if (strncmp(buffer, "Name", 4) == 0) {
                buffer[strlen(buffer) - 1] = '\0';  // Remove the newline character
                strcpy(processInfo[1], buffer + 6); // Copy the string without the title
            } else if (strncmp(buffer, "State", 5) == 0) {
                buffer[strlen(buffer) - 1] = '\0';
                strcpy(processInfo[2], buffer + 7);
            } else if (strncmp(buffer, "Uid", 3) == 0) {
                buffer[strlen(buffer) - 1] = '\0';
                extractFirst(buffer, processInfo[3]); // Places the first number into buffer
            } else if (strncmp(buffer, "Gid", 3) == 0) {
                buffer[strlen(buffer) - 1] = '\0';
                extractFirst(buffer, processInfo[4]);
            }
        }

        printf("%-15s\t%-15s\t%-15s\t%-15s\t%-15s\n", processInfo[0], processInfo[1], processInfo[2], processInfo[3], processInfo[4]);
        fclose(process);
    }
    return 0;
}
