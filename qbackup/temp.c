#include <stdio.h>

struct _cnt{
char key[16];
int samount;
int ramount;
int randomamount;
int directamount;
};

int main()
{
printf("%d",sizeof(struct _cnt));
}
