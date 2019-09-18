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

	while (*tmp != '\0') {
		if (isdigit(*tmp) != 0) {
			return 0;
		}	
	}
	return 1;	
}

int main(){
   struct dirent ** namelist;
   int numDirectories;
   
   numDirectories = scandir("/proc", &namelist, isProcessDir, NULL);
   
   // Check if no directories were read    
   if (numDirectories == -1) {
  	printf("The /proc directory could not be read"); 
	return 1;
   }
   printf("%d", numDirectories);
   return 0;

}
