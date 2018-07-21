#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>

struct _cnt{
char key[16];
int samount;
int ramount;
};

struct _cnt cnt;

char *cntfile = "../cntr.dat";

int main()
{
int handle;
if((handle=open(cntfile,O_RDWR))==-1)
	{printf("Unable to open %s",cntfile);
	exit(0);
	}
while(read(handle,&cnt,24)>0)
	{
	cnt.samount=0;
	cnt.ramount=0;
	lseek(handle,-24,SEEK_CUR);
	write(handle,&cnt,24);
	}
close(handle);
}