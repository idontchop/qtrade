#include <stdio.h>
#include <stdlib.h>

int main()
{
if(!getenv("HTTP_REFERER"))
	{
	printf("Content-type: text/html\n\n(null)");
	return 1;
	}
else printf("Content-type: text/html\n\n%s",getenv("HTTP_REFERER"));
return 1;
}
