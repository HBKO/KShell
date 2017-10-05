//获取主机信息的文件
#include<stdio.h>
#include<pwd.h>
#include<sys/types.h>
#include<string.h>
#include<unistd.h>
#include<stdlib.h>
#include "mystring.h"

/*从字符串的右边截取n个字符*/  
char * right(char *dst,char *src, int n)  
{  
    char *p = src;  
    char *q = dst;  
    int len = strlen(src);  
    if(n>len) n = len;  
    p += (len-n);   /*从右边第n个字符开始，到0结束，很巧啊*/  
    while((*(q++) = *(p++))); 
    return dst;  
}  


/* 获取字符串相关信息 */
struct hostmessage *getmessage()
{
    struct hostmessage* new=malloc(sizeof(struct hostmessage));
    new->cwd=malloc(MAXNAME);
    new->hostname=malloc(MAXNAME);
    new->usrname=malloc(MAXNAME);
    getcwd(new->cwd,MAXNAME);
    gethostname(new->hostname,MAXNAME);
    new->isroot=(geteuid()==getuid());
    struct passwd *my_info;
    my_info=getpwuid(getuid());
    memcpy(new->usrname,my_info->pw_name,strlen(my_info->pw_name)*sizeof(char));
    return new;
}


/* 获取shell命令字符串 */
void printname()
{
    //函数声明
    void shellcwd(char *cwd,char *dst);
    void shellhostname(char *res,char *dst);
    char *isroot="$";
    struct hostmessage* result=getmessage();
    char *cwd=malloc(sizeof(MAXNAME));
    char *hostname=malloc(sizeof(MAXNAME));
    shellcwd(result->cwd,cwd);
    shellhostname(result->hostname,hostname);
    if(result->isroot!=1) isroot="#";
    printf("[hekewen][%s@%s %s]%s",result->usrname,hostname,cwd,isroot);
    freemessage(result);
    free(cwd);
    free(hostname);
}

void shellhostname(char *res,char *dst)
{
    int i=0;
    while(res[i]!='\0' && res[i]!='.')
    {
        dst[i]=res[i];
        i++;
    }
}


void shellcwd(char *cwd,char *dst)
{
    int end=0;
    int size=strlen(cwd);
    int i=0;
    for(i=0;i<size;++i)
    {
        if(cwd[i]=='/')
        {
            end=0;
        }
        end++;
    }
    right(dst,cwd,end-1);
}




//1表示程序成功
int freemessage(struct hostmessage *oldmessage)
{
    free(oldmessage->cwd);
    free(oldmessage->hostname);
    free(oldmessage->usrname);
    free(oldmessage);
    return 1;
}





