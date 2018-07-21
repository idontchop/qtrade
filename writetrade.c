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

void main()
{
trade.amount=0;
strcpy(trade.url,"http://www.pornpub.com");
strcpy(trade.key,"DEFAULT");
handle=open("trade.dat",O_WRONLY|O_CREAT);
write(handle,&trade,144);
close(handle);
}
