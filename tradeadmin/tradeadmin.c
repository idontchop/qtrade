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

int cgiMain()
{
char command[20];
char oldkey[20];
printf("Content-type: text/html\n\n");
if((handle=open(tradefile,O_RDWR))==-1)
	{printf("Unable to open %s",tradefile);
	exit(0);}
if(cgiFormString("command",command,19)!=cgiFormSuccess)
	{printmenu();
	close(handle);
	exit(0);
	}
cgiFormString("oldkey",oldkey,19);
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
int alphanum;
int high;
int lasthigh;
struct _trade alpharead;
printf("<FORM METHOD=\"POST\" ACTION=\"tradeadmin.cgi\">\n");
printf("<INPUT TYPE=\"HIDDEN\" NAME=\"command\" VALUE=\"new\">\n");
printf("<TABLE BORDER=\"2\"><TR><TD>\n");
printf("Key: <INPUT TYPE=\"text\" NAME=\"newkey\" SIZE=\"15\" MAXLENGTH=\"15\"><BR>\n");
printf("Linkback URL: <INPUT TYPE=\"text\" NAME=\"url\" SIZE=\"50\" MAXLENGTH=\"120\"><BR>");
printf("Link Difference: <INPUT TYPE=\"text\" NAME=\"amount\" VALUE=\"0\" SIZE=\"8\" MAXLENGTH=\"8\">");
printf("</TD><TD><INPUT TYPE=\"SUBMIT\" VALUE=\"add trade\"></TD></TR></FORM></TABLE>");
high=0;
while(1)
{
lseek(handle,0,SEEK_SET);
lasthigh=999999999;
while(read(handle,&alpharead,144)>0)
{
alphanum=((alpharead.key[0]*100000)+(alpharead.key[1]*10000)+(alpharead.key[2]*1000)+(alpharead.key[3]*100)+(alpharead.key[4]*10)+(alpharead.key[5]*1));
if((alphanum>high)&&(alphanum<lasthigh))
	{
	strcpy(trade.key,alpharead.key);
	strcpy(trade.url,alpharead.url);
	trade.amount=alpharead.amount;
	lasthigh=alphanum;
	}
}
if(lasthigh==999999999) {break;}
else {high=lasthigh;}
printf("<FORM METHOD=\"POST\" ACTION=\"tradeadmin.cgi\">\n");
printf("<INPUT TYPE=\"HIDDEN\" NAME=\"command\" VALUE=\"edit\">\n");
printf("<TABLE BORDER=\"2\"><TR><TD><INPUT TYPE=\"HIDDEN\" NAME=\"oldkey\" VALUE=\"%s\">\n",trade.key);
printf("Key: <INPUT TYPE=\"text\" NAME=\"newkey\" VALUE=\"%s\" SIZE=\"15\" MAXLENGTH=\"15\"><BR>\n",trade.key);
printf("Linkback URL: <INPUT TYPE=\"text\" NAME=\"url\" VALUE=\"%s\" SIZE=\"50\" MAXLENGTH=\"120\">",trade.url);
printf("<BR>Link Difference: <INPUT TYPE=\"text\" NAME=\"amount\" VALUE=\"%d\" SIZE=\"8\" MAXLENGTH=\"8\">",trade.amount);
printf("</TD><TD><INPUT TYPE=\"SUBMIT\" VALUE=\"update\"></TD></TR></FORM></TABLE>");

}

}




