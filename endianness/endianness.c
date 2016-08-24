#include <stdio.h>
#include <string.h>
#include <malloc.h>
#include <errno.h>

main()
{
int a = 0x44332211;

char *c = (char *)&a;

if(*c == 0x11)
  printf("Its LE mach..\n");
else
  printf("Its BE mach..\n");
}
