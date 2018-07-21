#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>

struct _trade{
char key[16];
char url[121];
int amount;
};

struct _trade trade;
int handle;

int main()
{
handle=open("trade.dat",O_RDONLY);
while(read(handle,&trade,144)>0){
printf("%s\n%s\n%d\n\n",trade.key,trade.url,trade.amount);
}
close(handle);
}
