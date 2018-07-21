#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include "cgic.h"

struct _trade{
char key[16];
char url[121];
int amount;
};

struct _trade trade;
struct _trade tread;
int handle;

char *tradefile = "../trade.dat";

void printmenu(void);

int testforsites(void);

int cgiMain()
{
char command[20];
char oldkey[20];
int oursite = 0;
printf("Content-type: text/html\n\n");
if((handle=open(tradefile,O_RDWR))==-1)
	{printf("Unable to open %s",tradefile);
	exit(0);}
if(testforsites()==-1)
	{printf("<B>CORRECTING DATABASE! DO NOT DELETE REQUIRED TRADES!</B><BR><BR>");
	}
if(cgiFormString("command",command,19)!=cgiFormSuccess)
	{printmenu();
	close(handle);
	exit(0);
	}
cgiFormString("oldkey",oldkey,19);
if(strcmp(oldkey,"ndabutt.com")==0)
	oursite=1;
if(strcmp(oldkey,"sexedition.com")==0)
	oursite=1;
if(strcmp(oldkey,"fosp.com")==0)
	oursite=1;
if(strcmp(oldkey,"tosp.com")==0)
	oursite=1;
if(strcmp(oldkey,"teenmegasite")==0)
	oursite=1;
if(strcmp(oldkey,"illiciterotica")==0)
	oursite=1;
if(strcmp(oldkey,"vipxxxclub")==0)
	oursite=1;
if(strcmp(oldkey,"hardcorehouse")==0)
	oursite=1;
if(cgiFormString("url",trade.url,120)!=cgiFormSuccess)
	{printf("Something is wrong with the url, we need that");
	exit(0);
	}
if(cgiFormString("newkey",trade.key,17)!=cgiFormSuccess)
	{printf("Something is wrong with the key, we need that");
	exit(0);
	}
if(cgiFormInteger("amount",&trade.amount,0)!=cgiFormSuccess)
	{printf("Something is wrong with the key, we need that");
	exit(0);
	}
if(strcmp(command,"new")==0)
	{
	lseek(handle,0,SEEK_END);
	write(handle,&trade,144);
	}
else if(strcmp(command,"edit")==0)
	{
	lseek(handle,0,SEEK_SET);
	while(read(handle,&tread,144)>0)
		{
		if(strcmp(tread.key,oldkey)==0)
			{lseek(handle,-144,SEEK_CUR);
			if(oursite==1){
				trade.url=tread.url;
				trade.key=oldkey;}
			write(handle,&trade,144);
			break;}
		}
	}
else printf("Unrecognized command");
printf("We are done");
printf("\n<BR><FORM METHOD=\"POST\" ACTION=\"tradeadmin.cgi\">\n<INPUT TYPE=\"SUBMIT\" VALUE=\"Return\"></FORM>");

close(handle);
exit(0);
}

void printmenu(void)
{
printf("<FORM METHOD=\"POST\" ACTION=\"tradeadmin.cgi\">\n");
printf("<INPUT TYPE=\"HIDDEN\" NAME=\"command\" VALUE=\"new\">\n");
printf("<TABLE BORDER=\"2\"><TR><TD>\n");
printf("Key: <INPUT TYPE=\"text\" NAME=\"newkey\" SIZE=\"15\" MAXLENGTH=\"15\"><BR>\n");
printf("Linkback URL: <INPUT TYPE=\"text\" NAME=\"url\" SIZE=\"50\" MAXLENGTH=\"120\"><BR>");
printf("Link Difference: <INPUT TYPE=\"text\" NAME=\"amount\" VALUE=\"0\" SIZE=\"8\" MAXLENGTH=\"8\">");
printf("</TD><TD><INPUT TYPE=\"SUBMIT\" VALUE=\"add trade\"></TD></TR></FORM></TABLE>");

while(read(handle,&trade,144)>0)
{
printf("<FORM METHOD=\"POST\" ACTION=\"tradeadmin.cgi\">\n");
printf("<INPUT TYPE=\"HIDDEN\" NAME=\"command\" VALUE=\"edit\">\n");
printf("<TABLE BORDER=\"2\"><TR><TD><INPUT TYPE=\"HIDDEN\" NAME=\"oldkey\" VALUE=\"%s\">\n",trade.key);
printf("Key: <INPUT TYPE=\"text\" NAME=\"newkey\" VALUE=\"%s\" SIZE=\"15\" MAXLENGTH=\"15\"><BR>\n",trade.key);
printf("Linkback URL: <INPUT TYPE=\"text\" NAME=\"url\" VALUE=\"%s\" SIZE=\"50\" MAXLENGTH=\"120\">",trade.url);
printf("<BR>Link Difference: <INPUT TYPE=\"text\" NAME=\"amount\" VALUE=\"%d\" SIZE=\"8\" MAXLENGTH=\"8\">",trade.amount);
printf("</TD><TD><INPUT TYPE=\"SUBMIT\" VALUE=\"update\"></TD></TR></FORM></TABLE>");

}

}


void testforsites(void)
{
int sexed = 0;
int fosp = 0;
int ndabu = 0;
int tosp = 0;
int tms = 0;
int illic = 0;
int vipxx = 0;
int hardc = 0;
int returnv = 0;
lseek(handle,0,SEEK_SET);
while(read(handle,&tread,144)>0)
     {
     if((strcmp(tread.key,"ndabutt.com")==0)&&(strcmp(tread.key,"http://www.ndabutt.com")==0))
     	ndabu=1;
     if((strcmp(tread.key,"sexedition.com")==0)&&(strcmp(tread.key,"http://www.sexedition.com"))
     	sexed=1;
     if((strcmp(tread.key,"fosp.com")==0)&&(strcmp(tread.key,"http://www.fosp.com"))
     	fosp=1;
     if((strcmp(tread.key,"tosp.com")==0)&&(strcmp(tread.key,"http://www.tosp.com")==0))
     	tosp=1;
     if((strcmp(tread.key,"teenmegasite")==0)&&(strcmp(tread.key,"http://www.teenmegasite.com")==0))
     	tms=1;
     if((strcmp(tread.key,"illiciterotica")==0)&&(strcmp(tread.key,"http://www.illiciterotica.com")==0))
     	illic=1;
     if((strcmp(tread.key,"vipxxxclub")==0)&&(strcmp(tread.url,"http://www.vipxxxclub.com")==0))
     	vipxx=1;
     if((strcmp(tread.key,"hardcorehouse")==0)&&(strcmp(tread.url,"http://www.hardcorehousewives.com")==0))
     	hardc=1;
     }
if(ndabu==0)
	{returnv=-1;
	strcpy(tread.key,"ndabutt.com");
	strcpy(tread.url,"http://www.ndabutt.com");
	write(handle,&tread,144);
	}
if(sexed==0)
	{returnv=-1;strcpy(tread.key,"sexedition.com");strcpy(tread.url,"http://www.sexedition.com");
	write(handle,&tread,144);}
if(fosp==0)
	{returnv=-1;strcpy(tread.key,"fosp.com");strcpy(tread.url,"http://www.fosp.com");
	write(handle,&tread,144);}
if(tosp==0)
	{returnv=-1;strcpy(tread.key,"tosp.com");strcpy(tread.url,"http://www.tosp.com");
	write(handle,&tread,144);}
if(tms==0)
	{returnv=-1;strcpy(tread.key,"teenmegasite");strcpy(tread.url,"http://www.teenmegasite.com");
	write(handle,&tread,144);}
if(illic==0)
	{returnv=-1;strcpy(tread.key,"illiciterotica");strcpy(tread.url,"http://www.illiciterotica.com");
	write(handle,&tread,144);}
if(vipxx==0)
	{returnv=-1;strcpy(tread.key,"vipxxxclub");strcpy(tread.url,"http://www.vipxxxclub.com");
	write(handle,&tread,144);}
if(hardc==0)
	{returnv=-1;strcpy(tread.key,"hardcorehouse");strcpy(tread.url,"http://www.hardcorehousewives.com");
	write(handle,&tread,144);}
return returnv;
}
