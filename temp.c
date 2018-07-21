#include <stdio.h>

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

int main()
{
printf("%d\n",sizeof(struct _cnt));
printf("%d",sizeof(struct _outs));
}
