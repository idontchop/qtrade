#include <stdio.h>
#include <string.h>
#include <fcntl.h>
#include <stdlib.h>
#include <time.h>
#include "cgic.h"

char *tradefile = "trade.dat";
char *cntfile = "cntr.dat";

struct _cnt{
char key[16];
int samount;
int ramount;
int randomamount;
int directamount;
};

struct _cnt cnt;
void cntr(int a,char *thekey,char type);
void find_default(void);

struct _trade{
char key[16];
char url[121];
int amount;
};

struct _trade trade;
struct _trade def;
int handle;

int cgiMain()
{
char refurl[200];
char findkey[17];
int count;
int number;
int highest;
//default def...
strcpy(def.key,"DEFAULT");
strcpy(def.url,"http://www.pornpub.com");
def.amount = 0;
if((handle=open(tradefile,O_RDWR))==-1)
	{
	printf("Content-type: text/html\n\n");
	printf("hmmm...I was unable to open trade.dat");
	exit(0);}
//first find default
while(read(handle,&trade,144)>0)
	{
	if(strcmp(trade.key,"DEFAULT")==0)
		{
		strcpy(def.url,trade.url);
		def.amount = trade.amount;
		}
	}
if(cgiFormString("url",refurl,198)!=cgiFormSuccess)
	{strcpy(refurl,"ahk");
	}
if(cgiFormString("send",findkey,17)==cgiFormSuccess)
	{//must be a direct outlink
	lseek(handle,0,SEEK_SET);
	while(read(handle,&trade,144)>0)
		{
		if(strcmp(trade.key,findkey)==0)
			{//found it... send user to this url, update amount, and exit
			trade.amount--;
			lseek(handle,-144,SEEK_CUR);
			write(handle,&trade,144);
			if(strcmp(findkey,"DEFAULT")==0)
				find_default();
			else{
			cgiHeaderLocation(trade.url);
			cntr(-1,trade.key,'D');
			exit(0);}
			}
		}
	}
lseek(handle,0,SEEK_SET);
for(count=0,highest=0,number=0;count<32000;count++)
	{
	if(read(handle,&trade,144)<1)
		break;
	if(strstr(refurl,trade.key)!=0)
		continue;
	if(strcmp(trade.key,"DEFAULT")==0)
		continue;
	if(strcmp(trade.key,"DELETED")==0)
		continue;
	if(trade.amount>highest)
		{number=count;
		highest=trade.amount;
		}
	}
lseek(handle,0,SEEK_SET);
if(highest==0)
	{
	while(read(handle,&trade,144)>0)
		{
		if(strcmp(trade.key,"DEFAULT")==0)
			{
			lseek(handle,-144,SEEK_CUR);
			trade.amount--;
			write(handle,&trade,144);
			break;
			}
		}
	find_default();
	}
else {	lseek(handle,(number*144),SEEK_SET);
	read(handle,&trade,144);
	trade.amount--;
	lseek(handle,-144,SEEK_CUR);
	write(handle,&trade,144);
	cgiHeaderLocation(trade.url);
	cntr(-1,trade.key,'N');
	}
close(handle);
}

void find_default(void)
{
int rhandle;
char randomyn[120];
int percentage;
char durl[80];
int chance;
int max = 0;
int nextmax = 0;
char currentkey[18];
time_t t;
srand((unsigned) time(&t));
sscanf(def.url,"%s",&randomyn);
if(strcasecmp(randomyn,"RANDOM")!=0)
	{
	cgiHeaderLocation(def.url);
	return;
	}
sscanf(def.url,"%s %d %d %s",&randomyn,&percentage,&chance,&durl);
if((rhandle=open(cntfile,O_RDWR))==-1)
	{
	cgiHeaderLocation(durl);
	cntr(1,"DEFAULT",'N');
	return;
	}
if(percentage>(rand()%101))
	{
	cgiHeaderLocation(durl);
	cntr(1,"DEFAULT",'N');
	return;
	}
//find max
lseek(rhandle,0,SEEK_SET);
while(read(rhandle,&cnt,32)>1)
	{
	if(cnt.ramount>max)
		max = cnt.ramount;
	}
lseek(rhandle,0,SEEK_SET);
if(chance>(rand()%100))
	{nextmax=max;
	}
else{while(1)
	{
	if(read(rhandle,&cnt,32)<1)
		{
		if((chance>(rand()%100))||(nextmax==0))
			break;
		lseek(rhandle,0,SEEK_SET);
		max = nextmax;
		nextmax = 0;
		continue;
		}
	if((cnt.ramount>nextmax)&&(cnt.ramount<max))
		nextmax=cnt.ramount;
	
	}}
//find key
if(nextmax==0)
	{
	cgiHeaderLocation(durl);
	cntr(1,"DEFAULT",'N');
	close(rhandle);
	}
else{
lseek(rhandle,0,SEEK_SET);
while(read(rhandle,&cnt,32)>1)
	{
	if(cnt.ramount==nextmax)
		strcpy(currentkey,cnt.key);
	}
close(rhandle);
//find url
lseek(handle,0,SEEK_SET);
while(read(handle,&trade,144)>1)
	{
	if(strcmp(currentkey,trade.key)==0)
		break;
	}
cgiHeaderLocation(trade.url);
cntr(1,"DEFAULT",'R');
cntr(-1,trade.key,'R');
}
}	

void cntr(int a,char *thekey,char type)
{
int rhandle;
int yep;
yep = 0;
// for default: thekey="default"  type='N' for hit to default url, type='R' for hit to a site

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
