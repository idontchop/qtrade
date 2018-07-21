#include <stdio.h>

int main()
{
char *url = "RANDOM 100 http://www.pornpub.com";
char randomyn[120];
int percentage;
char urld[120];
sscanf(url,"%s %d %s",&randomyn,&percentage,&urld);
printf("\nrandomyn: %s\npercentage: %d\nurl: %s\n",randomyn,percentage,urld);
}
