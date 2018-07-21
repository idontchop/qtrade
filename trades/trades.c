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
struct _outs oc;

char *cntfile = "../cntr.dat";
char *outfile = "../outc.dat";
char *ocfile = "../outcookie.dat";

int main()
{
int handle;
int ohandle;
int ochandle;
int stotal = 0;
int rtotal = 0;
int directtotal=0;
int randomtotal=0;
int eachtotal;
int ctotal;
int ocamount;
int octotal=0;
printf("Content-type: text/html\n\n");
if((handle=open(cntfile,O_RDONLY))==-1)
	{printf("Unable to open %s",cntfile);
	exit(0);}
if((ochandle=open(ocfile,O_RDONLY))==-1)
	{printf("Unable to open %s",ocfile);
	exit(0);}
	
printf("<HTML><HEAD><TITLE>Our trades</TITLE><META HTTP-EQUIV=\"Refresh\" CONTENT=60 ></HEAD>\n");
printf("<BODY><BR>\n");
printf("<TABLE BORDER=\"1\"><TR><TD COLSPAN=\"7\"><CENTER><B>Our traffic trades</B></CENTER></TD></TR>\n");
printf("<TR><TD>Site key</TD><TD ALIGN=RIGHT># hits<BR>we received</TD><TD ALIGN=RIGHT># of outclicks<BR> from site<BR>(next version)</TD><TD ALIGN=RIGHT># hits<BR> we sent</TD><TD ALIGN=RIGHT># random hits<BR> we sent</TD><TD ALIGN=RIGHT># direct hits<BR> we sent</TD><TD ALIGN=RIGHT># Total hits<BR> we sent</TD></TR>\n");
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
	ocamount=0;
	lseek(ochandle,0,SEEK_SET);
	while(read(ochandle,&oc,44)>0)
		{
		if(strcmp(cnt.key,oc.key)==0)
			{ocamount=oc.amount;
			break;}
		}
	eachtotal = 0;
	stotal += cnt.samount;
	rtotal += cnt.ramount;
	directtotal += cnt.directamount;
	randomtotal += cnt.randomamount;
	octotal += ocamount;
	eachtotal = (cnt.samount + cnt.directamount + cnt.randomamount);
	printf("<TR><TD>%s</TD><TD ALIGN=RIGHT>%d</TD><TD ALIGN=RIGHT>%d</TD><TD ALIGN=RIGHT>%d</TD><TD ALIGN=RIGHT>%d</TD><TD ALIGN=RIGHT>%d</TD><TD ALIGN=RIGHT>%d</TD></TR>",cnt.key,cnt.ramount,ocamount,cnt.samount,cnt.randomamount,cnt.directamount,eachtotal);
	}
ctotal=(stotal+directtotal+randomtotal);
printf("<TR><TD><B>TOTALS</B></TD><TD ALIGN=RIGHT><B>%d</B></TD><TD ALIGN=RIGHT><B>%d</B></TD><TD ALIGN=RIGHT><B>%d</B></TD><TD ALIGN=RIGHT><B>%d</B></TD><TD ALIGN=RIGHT><B>%d</B></TD><TD ALIGN=RIGHT><B>%d</B></TD></TR>",rtotal,octotal,stotal,randomtotal,directtotal,ctotal);
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
lseek(ochandle,0,SEEK_SET);
printf("<BR>Outgoing clicks from other than trades:<BR><TABLE BORDER=\"1\">");
while(read(ochandle,&oc,44)>0)
	{
	if(strcmp(oc.key,"nocookie")==0)
		{printf("<TR><TD>No Cookie</TD><TD ALIGN=RIGHT>%d</TD></TR>",oc.amount);
		}
	if(strcmp(oc.key,"NONE")==0)
		{printf("<TR><TD>No refer from trade</TD><TD ALIGN=RIGHT>%d</TD></TR>",oc.amount);
		}
	}	
printf("</TABLE><BR><BR>");	
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
printf("</TABLE></BODY></HTML>");
close(ohandle);
close(ochandle);
}
