#include <stdio.h>
#include <string.h>
#include <fcntl.h>
#include <stdlib.h>
#include <time.h>
#include "cgic.h"

char *tradefile = "trade.dat";
char *cntfile = "cntr.dat";
char *outfile = "outc.dat";
char *ocfile = "outcookie.dat";

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

struct _outs outs;
struct _cnt cnt;
struct _outs outcookie;
void cntr(int a,char *thekey,char type);
void find_default(void);
char getcookie(char *key,char *value);
void outlink(void);
void outcookies(void);

struct _trade{
char key[16];
char url[121];
int amount;
};

struct _trade trade;
struct _trade def;
int handle;
char tokey[15];
char visitedcookie[200];

int cgiMain()
{
char refurl[200];
char findkey[17];
char whichpage[4];
int count;
int number;
int highest;
int p;
//update out link
outlink();
//log out cookie link
//outcookies();
//default def...
strcpy(def.key,"DEFAULT");
strcpy(def.url,"http://www.pornpub.com");
def.amount = 0;
if(getcookie("visited",visitedcookie)==1) strcpy(visitedcookie,"none");
//report whichpage
if(getcookie("whichpage",whichpage)==1)
	cntr(3,"Outgoing",'C');
else 	{
	p=atoi(whichpage);
	if(p>2)
		p=2;
	cntr(p,"Outgoing",'C');
	}
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
	if(strstr(visitedcookie,trade.key)!=0)
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
	printf("Location: %s\n",trade.url);
	if(strcmp(visitedcookie,"none")==0)
		printf("Set-Cookie: visited=%s;path=/\n\n",trade.key);
	else printf("Set-Cookie: visited=%s %s;path=/\n\n",visitedcookie,trade.key);
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
	if((cnt.ramount>max)&&(strstr(visitedcookie,cnt.key)==0))
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
	if((cnt.ramount>nextmax)&&(cnt.ramount<max)&&(strstr(visitedcookie,cnt.key)==0))
		nextmax=cnt.ramount;
	
	}
	}
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
printf("Location: %s\n",trade.url);
if(strcmp(visitedcookie,"")==0)
	printf("Set-Cookie: visited=%s;path=/\n\n",trade.key);
else if(strcmp(visitedcookie,"none")==0)
	printf("Set-Cookie: visited=%s;path=/\n\n",trade.key);
else printf("Set-Cookie: visited=%s %s;path=/\n\n",visitedcookie,trade.key);
cntr(1,"DEFAULT",'R');
cntr(-1,trade.key,'R');
}
}	


char getcookie(char *key,char *value)
{
int count = -1;
int cc;
char *cookie;
char test[40];
int maxcount;
cookie = (char *)getenv("HTTP_COOKIE");
if(!cookie)
   return 1;
maxcount=strlen(cookie);
while(1){
for(cc=0,count++;count<maxcount;count++,cc++)
	{
	if(cookie[count]=='=')
		{test[cc]='\0';
		break;}
	else test[cc]=cookie[count];
	}	
for(cc=0,count++;count<maxcount;count++,cc++)
	{
	if(cookie[count]==';')
		{value[cc]='\0';
		count++;
		break;}
	else value[cc]=cookie[count];	
	}
if(count==maxcount)
	value[cc]='\0';
if(strcmp(key,test)==0)
	return 0;
if(count==maxcount)
	break;
}
return 1;
}

//type - N - normal    R - Random    D - Direct
//type - C - cookie, increase number referred to by 1
//samount = 0, ramount = 1, directamount = 2, randomamount = 3
void cntr(int a,char *thekey,char type)
{
int rhandle;
int yep;
yep = 0;
if((rhandle=open(cntfile,O_RDWR))==-1)
	{
	return;
	}
if(type=='C')//do cookie
	{
	while(read(rhandle,&cnt,32)>0)
		{
		if(strcmp(cnt.key,thekey)==0)
			{
			switch(a)
			{
			case 0:cnt.samount++;break;
			case 1:cnt.ramount++;break;
			case 2:cnt.directamount++;break;
			case 3:cnt.randomamount++;break;
			}
			yep=1;
			lseek(rhandle,-32,SEEK_CUR);
			write(rhandle,&cnt,32);
			}
		}
	if(yep==0)
		{
		strcpy(cnt.key,thekey);
		cnt.samount=0;
		cnt.ramount=0;
		cnt.directamount=0;
		cnt.randomamount=0;
			switch(a)
			{
			case 0:cnt.samount++;break;
			case 1:cnt.ramount++;break;
			case 2:cnt.directamount++;break;
			case 3:cnt.randomamount++;break;
			}
		write(rhandle,&cnt,32);
		}	
	}
else{
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
     }
close(rhandle);
}

void outlink(void)
{
int ohandle;
char out[41];
int yep;
yep=0;
switch(cgiFormString("link",out,40))
	{
	case cgiFormSuccess:
	case cgiFormTruncated:break;
	default: return;
	}
if((ohandle=open(outfile,O_RDWR))==-1)
	return;
while(read(ohandle,&outs,44)>0)
	{
	if(strcmp(outs.key,out)==0)
		{
		outs.amount++;
		lseek(ohandle,-44,SEEK_CUR);
		write(ohandle,&outs,44);
		yep=1;
		}
	}
if(yep==0)
	{strcpy(outs.key,out);
	outs.amount=1;
	write(ohandle,&outs,44);
	}
close(ohandle);
}

void outcookies(void)
{
char camefrom[15];
int ochandle;
int yep;
yep = 0;
if(getcookie("camefrom",camefrom)==1)
	{strcpy(camefrom,"nocookie");
	}
if((ochandle=open(ocfile,O_RDWR))==-1)
	return;
while(read(ochandle,&outcookie,44)>0)
	{
	if(strcmp(outcookie.key,camefrom)==0)
		{
		outcookie.amount++;
		lseek(ochandle,-44,SEEK_CUR);
		write(ochandle,&outcookie,44);
		yep=1;
		}
	}
if(yep==0)
	{
	strcpy(outcookie.key,camefrom);
	outcookie.amount=1;
	write(ochandle,&outcookie,44);
	}
close(ochandle);
}
