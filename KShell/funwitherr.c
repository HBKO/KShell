//引用的头文件
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/time.h>
#include <errno.h>
#include <string.h>
#include "funwitherr.h"

/* 对fork()的警告进行封装
   对一个fork的包装函数，它随机地决定父进程和子进程执行的顺序。父进程和
   子进程扔一枚硬币来决定谁会休眠，因而给另一个进程被调度的机会
   及尽可能的产生竞争条件的情况，来进行问题检测 */
pid_t Fork(void)
{
    static struct timeval time;
    unsigned boo1,secs;
    pid_t pid;

    /* Generate a different seed each time the function is called */
    //获取当前时间，利用当前时间来产生随机种子
    gettimeofday(&time,NULL);
    srand(time.tv_usec);

    /* Determine whether to sleep in parent of child and for how long */
    //决定那个进程睡眠
    boo1=(unsigned)(CONVERT(rand())+0.5);
    //决定该进程睡眠多久
    secs=(unsigned)(CONVERT(rand())*MAX_SLEEP);

    /* Call the real fork function */
    if((pid=fork())<0)
        return pid;
    
    if(pid==0) //Child
    {
        if(boo1)
        {
            usleep(secs);
        }
    }
    else    //Parent
    {
        if(!boo1)
        {
            usleep(secs);
        }
    }

    /* Return the PID like a normal fork call */
    return pid;
}


/* 针对sigemptyset进行警告封装 */
void Sigemptyset(sigset_t *mask)
{
    if(sigemptyset(mask)!=0)
        unix_error("sigemptyset error");
    
}



/* 针对sigaddset()进行警告封装 */
void Sigaddset(sigset_t *mask,int signum)
{
    if(sigaddset(mask,signum)!=0)
        unix_error("sigaddset error!");
}

/* 针对sigprocmask()进行警告封装 */
void Sigprocmask(int how, const sigset_t * set, sigset_t * oldset)
{
    if(sigprocmask(how,set,oldset)!=0)
        unix_error("sigprocmask error!");
}


/* execve with error */
void Execve(char *string,char **argv,char ** environ)
{
    if(execve(argv[0],argv,environ)<0) 
    {   
        printf("%s: Command not found.\n",argv[0]);
        exit(0);
    }   
}


/*
 * unix_error - unix-style error routine
 */
void unix_error(char *msg)
{
    fprintf(stdout, "%s: %s\n", msg, strerror(errno));
    exit(1);
}

/*
 * app_error - application-style error routine
 */
void app_error(char *msg)
{
    fprintf(stdout, "%s\n", msg);
    exit(1);
}