#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include "mystring.h"
#define MAXLINE 100*sizeof(char)



int main()
{
    init_rl();
    char* cmdline=NULL;
    char* test=malloc(MAXLINE);
    getallname(test);
    while(1)
    {
    	cmdline=rl_gets(test);
	    if(strcmp(cmdline,"quit")==0)
	        break;
    }
    free(test);
    return 0;
}
