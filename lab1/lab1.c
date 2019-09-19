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
        printf("The /proc directory could not be read");
        return 1;
    }

    int i;
    for (i = 0; i < numDirectories; i++) {
        char path[100];
        sprintf(path, "/proc/%s/status", namelist[i]->d_name);
        printf("%s\n", path);
    }

    // printDirectories(namelist, numDirectories);
    return 0;
}
