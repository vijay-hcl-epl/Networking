#include <stdio.h>
#include <string.h>
#include <malloc.h>
#include <errno.h>

main()
{
unsigned int a = 0x77889911;

#if METHOD1
unsigned int b = 0;
unsigned int ans = 0;
int i = 0, j = 0;

for(i = 3, j = 0; i >= 0; i--,j++)
{
b = a << (i * 8);
b = b & 0xff000000;
b = b >> (j * 8);
ans |= b;
}
printf("The val of a   is %x\n", a);
printf("The val of ans is %x\n", ans);
#else

char *cptr = (char *)&a;
char ans[4] = {0};
int i = 0;

int *iptr = (int *) ans;

for(i = 3; i >= 0; i--)
{
    ans[i] = *cptr;
    cptr++;
}
printf("The val of a   is %x\n", a);
printf("The val of ans is %x\n", *iptr);
#endif

}
