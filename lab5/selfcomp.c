#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>

void doTest();

int main(int argc, char * argv[]){

    /* call the vulnerable function */
    doTest();

    exit(0);
}

char compromise[45] = {
    'h','i'
};


char * compromise1 =
    "xxxxxxxxxxxxxxxxxxxx"
    "xxxxxxxxxxxxxxxxxxxx"
    "xxxxxxxxxxxxxxxxxxxx"
    "WXYZ";

int i;
void doTest(){
    char buffer[120];
    /* copy the contents of the compromise
       string onto the stack
       - change compromise1 to compromise
         when shell code is written */

    for (i = 0; compromise1[i]; i++)
	buffer[i] = compromise1[i];
}

