#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>

struct _trade{
char key[16];
char url[121];
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
void cntr(int a,char *thekey,char type);

struct _trade trade;
int handle;

char *cntfile = "../cgi-bin/cntr.dat";
char *tradefile = "../cgi-bin/trade.dat";

int main()
{
char *referer;
printf("Content-type: text/html\n\n");
referer = getenv("HTTP_REFERER");
if((handle=open(tradefile,O_RDWR))==-1)
	{
	exit(0);}
if(!referer)
	{
	exit(0);}
while(read(handle,&trade,144)>0)
	{
	if(strstr(referer,trade.key)!=0)
		{
		trade.amount++;
		lseek(handle,-144,SEEK_CUR);
		if(write(handle,&trade,144)<1)
			printf("write unsuccessful");
		cntr(1,trade.key,'N');
		break;
		}
	}
close(handle);
}

//type - N - normal    R - Random    D - Direct
void cntr(int a,char *thekey,char type)
{
int rhandle;
int yep;
yep = 0;
if((rhandle=open(cntfile,O_RDWR))==-1)
	{
	return;
	}
while(read(rhandle,&cnt,32)>0)
	{
	if(strcmp(cnt.key,thekey)==0)
		{
		lseek(rhandle,-32,SEEK_CUR);
		if(type=='D')
			{cnt.directamount++;}
		else if(type=='R')
			{cnt.randomamount++;}
		else	{
		if(a < 0)
			cnt.samount++;
		else cnt.ramount++;
			}
		write(rhandle,&cnt,32);
		yep=1;
		break;
		}
	}
if(yep==0)
	{
	strcpy(cnt.key,thekey);
	cnt.samount=0;
	cnt.ramount=0;
	cnt.directamount=0;
	cnt.randomamount=0;
	if(type == 'D')
		{cnt.directamount++;
		}
	else if(type == 'R')
		{cnt.randomamount++;
		}
	else {
	if(a < 0)
		cnt.samount=1;
	else cnt.ramount=1;
		}
	write(rhandle,&cnt,32);
	}
close(rhandle);
}
