//获取主机信息的文件
#include<stdio.h>
#include<pwd.h>
#include<sys/types.h>
#include<string.h>
#include<unistd.h>
#include<stdlib.h>
#include <readline/readline.h>  
#include <readline/history.h>
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
    memset(new->usrname,0,MAXNAME);
    memcpy(new->usrname,my_info->pw_name,strlen(my_info->pw_name)*sizeof(char));
    return new;
}


/* 获取shell命令字符串 */
void getallname(char* name)
{
    //函数声明
    void shellcwd(char *cwd,char *dst);
    void shellhostname(char *res,char *dst);
    char isroot='$';
    struct hostmessage* result=getmessage();
    char *cwd=malloc(sizeof(MAXNAME));
    char *hostname=malloc(sizeof(MAXNAME));
    char res[100];
    //对每次传入的数组进行清空
    if(name)
        memset(name,0,MAXNAME);
    shellcwd(result->cwd,cwd);
    shellhostname(result->hostname,hostname);
    if(result->isroot!=1) isroot='#';
    sprintf(res,"[KShell][%s@%s %s]%c",result->usrname,hostname,cwd,isroot);
    fflush(stdout);
    memcpy(name,res,strlen(res)*sizeof(char));
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


//初始化readline
void init_rl()
{
    rl_bind_key('\t', rl_complete);
}


char *rl_gets(const char* prompt)
{
    static char *line_read=(char *)NULL;
  /* If the buffer has already been allocated,
     return the memory to the free pool. */
  if (line_read)
    {
      free (line_read);
      line_read = (char *)NULL;
    }

  /* Get a line from the user. */
  line_read = readline(prompt);

  /* If the line has any text in it,
     save it on the history. */
  if (line_read && *line_read)
    add_history (line_read);

  return (line_read);
}


