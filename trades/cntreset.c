#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>

struct _outs{
char key[40];
int amount;
};

struct _cnt{
char key[16];
int samount;
int ramount;
int randomamount;
int directamount;
};

struct _cnt cnt;
struct _outs outs;

char *cntfile = "../cntr.dat";
char *outfile = "../outc.dat";

int main()
{
int handle;
int ohandle;
if((handle=open(cntfile,O_RDWR))==-1)
	{printf("Unable to open %s",cntfile);
	exit(0);
	}
while(read(handle,&cnt,32)>0)
	{
	cnt.samount=0;
	cnt.ramount=0;
	cnt.randomamount=0;
	cnt.directamount=0;
	lseek(handle,-32,SEEK_CUR);
	write(handle,&cnt,32);
	}
close(handle);
if((ohandle=open(outfile,O_RDWR))==-1)
	{printf("Unable to open %s",outfile);
	exit(0);
	}
while(read(handle,&outs,44)>0)
	{
	outs.amount=0;
	lseek(ohandle,-44,SEEK_CUR);
	write(ohandle,&outs,44);
	}
close(ohandle);

}


