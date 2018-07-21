#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>

struct _cnt{
char key[16];
int samount;
int ramount;
int randomamount;
int directamount;
};

struct _cnt cnt;
int handle;


int main()
{
if((handle=open("../cntr.dat",O_RDONLY))==-1)
	{
	printf("unable to open cntr.dat");
	}
while(read(handle,&cnt,32)>0)
	{
	printf("key: %s\nsamount: %d\nramount: %d\nrandom: %d\ndirect: %d\n\n",cnt.key,cnt.samount,cnt.ramount,cnt.randomamount,cnt.directamount);
	}
close(handle);
}