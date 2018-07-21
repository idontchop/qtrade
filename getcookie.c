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