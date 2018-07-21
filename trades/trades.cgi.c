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

struct _outs outs;
struct _cnt cnt;

char *cntfile = "../cntr.dat";
char *outfile = "../outc.dat";

int main()
{
int handle;
int ohandle;
int stotal = 0;
int rtotal = 0;
int directtotal=0;
int randomtotal=0;
int eachtotal;
int ctotal;
printf("Content-type: text/html\n\n");
if((handle=open(cntfile,O_RDONLY))==-1)
	{printf("Unable to open %s",cntfile);
	exit(0);}
printf("<HTML><HEAD><TITLE>Our trades</TITLE><META HTTP-EQUIV=\"Refresh\" CONTENT=60 ></HEAD>\n");
printf("<BODY><BR>\n");
printf("<TABLE BORDER=\"1\"><TR><TD COLSPAN=\"6\"><CENTER><B>Our traffic trades</B></CENTER></TD></TR>\n");
printf("<TR><TD>Site key</TD><TD ALIGN=RIGHT># hits we sent to equal</TD><TD ALIGN=RIGHT># hits we received</TD><TD ALIGN=RIGHT># random hits we sent</TD><TD ALIGN=RIGHT># direct hits we sent</TD><TD ALIGN=RIGHT># Total hits we sent</TD></TR>\n");
while(read(handle,&cnt,32)>0)
	{
	if(strcmp(cnt.key,"DEFAULT")==0)
		continue;
	if(strcmp(cnt.key,"Whichpage")==0)
		continue;
	if(strcmp(cnt.key,"Hits")==0)
		continue;
	if(strcmp(cnt.key,"Outgoing")==0)
		continue;
	if((cnt.samount==0)&&(cnt.ramount==0)&&(cnt.randomamount==0)&&(cnt.directamount==0))
		continue;
	eachtotal = 0;
	stotal += cnt.samount;
	rtotal += cnt.ramount;
	directtotal += cnt.directamount;
	randomtotal += cnt.randomamount;
	eachtotal = (cnt.samount + cnt.directamount + cnt.randomamount);
	printf("<TR><TD>%s</TD><TD ALIGN=RIGHT>%d</TD><TD ALIGN=RIGHT>%d</TD><TD ALIGN=RIGHT>%d</TD><TD ALIGN=RIGHT>%d</TD><TD ALIGN=RIGHT>%d</TD></TR>",cnt.key,cnt.samount,cnt.ramount,cnt.randomamount,cnt.directamount,eachtotal);
	}
ctotal=(stotal+directtotal+randomtotal);
printf("<TR><TD><B>TOTALS</B></TD><TD ALIGN=RIGHT><B>%d</B></TD><TD ALIGN=RIGHT><B>%d</B></TD><TD ALIGN=RIGHT><B>%d</B></TD><TD ALIGN=RIGHT><B>%d</B></TD><TD ALIGN=RIGHT><B>%d</B></TD></TR>",stotal,rtotal,randomtotal,directtotal,ctotal);
printf("\n</TABLE>");
lseek(handle,0,SEEK_SET);
while(read(handle,&cnt,32)>0)
	{
	if(strcmp(cnt.key,"DEFAULT")==0)
		{
		printf("<TABLE>\n");
		printf("<TR><TD># hits to default url</TD><TD># hits to random sites</TD></TR>");
		printf("<TR><TD ALIGN=RIGHT>%d</TD><TD ALIGN=RIGHT>%d</TD></TR>\n",cnt.ramount,cnt.randomamount);
		printf("</TABLE>\n");
		}
	}
printf("</BODY></HTML>");
lseek(handle,0,SEEK_SET);
while(read(handle,&cnt,32)>0)
	{
	if(strcmp(cnt.key,"Whichpage")==0)
		{
		printf("<TABLE BORDER=\"1\"><TR><TD>First Visit</TD><TD>Second Visit</TD><TD>3rd Visit</TD><TD>4th+ visit</TD></TR>");
		printf("<TR><TD>%d</TD><TD>%d</TD><TD>%d</TD><TD>%d</TD></TR></TABLE>",cnt.samount,cnt.ramount,cnt.directamount,cnt.randomamount);
		}
	}
lseek(handle,0,SEEK_SET);
while(read(handle,&cnt,32)>0)
	{
	if(strcmp(cnt.key,"Outgoing")==0)
		{
		printf("<BR>Outoing clicks on which visit:<BR>");
		printf("<TABLE BORDER=\"1\"><TR><TD>First Visit</TD><TD>Second Visit</TD><TD>3rd+ Visit</TD><TD>Never hit index page or cookies disabled</TD></TR>");
		printf("<TR><TD>%d</TD><TD>%d</TD><TD>%d</TD><TD>%d</TD></TR></TABLE>",cnt.samount,cnt.ramount,cnt.directamount,cnt.randomamount);
		}
	}
lseek(handle,0,SEEK_SET);
while(read(handle,&cnt,32)>0)
	{
	if(strcmp(cnt.key,"Hits")==0)
		{
		printf("<BR>Total number of root index hits: %d<BR>",cnt.samount);
		printf("Number of hits where trade referrer not found: %d<BR>",cnt.randomamount);
		}
	}
close(handle);
printf("<BR><BR>Outgoing links clicked:<BR>");
if((ohandle=open("../outc.dat",O_RDWR))==-1)
	{
	printf("None found - unable to open outc.dat");
	}
else{
printf("<TABLE BORDER=\"1\"><TR><TD><B>Name</B></TD><TD><B>Amount</B></TD></TR>");
while(read(ohandle,&outs,44)>0)
		{
		printf("<TR><TD>%s</TD><TD ALIGN=RIGHT>%d</TD></TR>",outs.key,outs.amount);
		}
	}
close(ohandle);
}
