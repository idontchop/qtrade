#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <fcntl.h>

int maxrotate = 1;
char *ext = "shtml";


char getcookie(char *key,char *value);
void setuniquecookie(void);

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

char from[15];

int main()
{
char page[5];
int p;
char send[20];
char *referer;
int foundit;
foundit=0;
//give one hit always
cntr(0,"Hits",'C');
strcpy(from,"none");
referer = getenv("HTTP_REFERER");
if(!referer);
else{
if((handle=open(tradefile,O_RDWR))==-1);
else {while(read(handle,&trade,144)>0)
	{
	if(strstr(referer,trade.key)!=0)
		{
		trade.amount++;
		foundit=1;
		strcpy(from,trade.key);
		lseek(handle,-144,SEEK_CUR);
		write(handle,&trade,144);		
		cntr(1,trade.key,'N');
		break;
		}
	}
	if(foundit==0)
		cntr(3,"Hits",'C');
	}
close(handle);
}

// Now point to correct page
if(getcookie("whichpage",page)==1)
	{
	printf("Location: index.%s\nSet-Cookie: whichpage=0;path=/;expires=Mon, 01-01-2020 01:01:01 GMT\n",ext);
	cntr(0,"Whichpage",'C');
	setuniquecookie();
	printf("\n");
	exit(0);
	}
else {p=atoi(page);
	if(p==(maxrotate-1))
		{p=0;
		cntr(3,"Whichpage",'C');}
	else {p++;
		if(p>2)
			cntr(3,"Whichpage",'C');
		else cntr(p,"Whichpage",'C');
		}
	if(p==0)
		sprintf(send,"index.%s",ext);
	else sprintf(send,"index%d.%s",p,ext);
	
     }
printf("Location: %s\nSet-Cookie: whichpage=%d;path=/\n",send,p);
setuniquecookie();
printf("\n");
return 0;
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
//generic stats: samount=hits to page   randomamount=couldn't find referrer
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

void setuniquecookie(void)
{
char string[200];
//first get key where visitor came from

if(getcookie("visited",string)==1)
	{
	if(strcmp(from,"none")!=0)
		{printf("Set-Cookie: visited=%s;path=/\n",from);
		printf("Set-Cookie: camefrom=%s;path=/\n",from);}
	else printf("Set-Cookie: camefrom=NONE;path=/\n");
	}
else	{
	if(strcmp(from,"none")!=0)
		{printf("Set-Cookie: visited=%s %s;path=/\n",string,from);
		printf("Set-Cookie: camefrom=%s;path=/\n",from);}
	else printf("Set-Cookie: camefrom=NONE;path=/\n");
	}
}
