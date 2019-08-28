#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <stdlib.h>
#include <ctype.h>
#include <sys/ioctl.h>
#include <unistd.h>
#include <time.h>


int main(int argc, char* argv)
{
        char* c = "abcdefghijklmnopqrstuvwxyz";
        for(int i =0; i<strlen(c); i++)
        {
                printf("%c, ascii: %d\n",c[i], (int) c[i]);
        }






}
