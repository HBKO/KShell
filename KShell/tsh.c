/* 
 * tsh - A tiny shell program with job control
 * 
 * <Put your name and login ID here>
 */
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <ctype.h>
#include <signal.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/time.h>
#include <errno.h>
#include "funwitherr.h"
#include "jobs.h"




/* Misc manifest constants */
#define MAXLINE    1024   /* max line size */
#define MAXARGS     128   /* max args on a command line */


/* Global variables */
extern char **environ;      /* defined in libc */
char prompt[] = "tsh> ";    /* command line prompt (DO NOT CHANGE) */
//extern int verbose;            /* if true, print additional output */
char sbuf[MAXLINE];         /* for composing sprintf messages */






/* End global variables */




/* Function prototypes */

/* Here are the functions that you will implement */
void eval(char *cmdline);
int builtin_cmd(char **argv);
void do_bgfg(char **argv);
void waitfg(pid_t pid);

void sigchld_handler(int sig);
void sigtstp_handler(int sig);
void sigint_handler(int sig);

/* Here are helper routines that we've provided for you */
int parseline(const char *cmdline, char **argv); 
void sigquit_handler(int sig);

void usage(void);

typedef void handler_t(int);
handler_t *Signal(int signum, handler_t *handler);

/*
 * main - The shell's main routine 
 */
int main(int argc, char **argv) 
{
    char c;
    char cmdline[MAXLINE];
    int emit_prompt = 1; /* emit prompt (default) */

    /* Redirect stderr to stdout (so that driver will get all output
     * on the pipe connected to stdout) */
    dup2(1, 2);

    /* Parse the command line */
    while ((c = getopt(argc, argv, "hvp")) != EOF) {
        switch (c) {
        case 'h':             /* print help message */
            usage();
	    break;
        case 'v':             /* emit additional diagnostic info */
            verbose = 1;
	    break;
        case 'p':             /* don't print a prompt */
            emit_prompt = 0;  /* handy for automatic testing */
	    break;
	default:
            usage();
	}
    }

    /* Install the signal handlers */

    /* These are the ones you will need to implement */
    Signal(SIGINT,  sigint_handler);   /* ctrl-c */
    Signal(SIGTSTP, sigtstp_handler);  /* ctrl-z */
    Signal(SIGCHLD, sigchld_handler);  /* Terminated or stopped child */

    /* This one provides a clean way to kill the shell */
    Signal(SIGQUIT, sigquit_handler); 

    /* Initialize the job list */
    initjobs(jobs);

    /* Execute the shell's read/eval loop */
    while (1) {

	/* Read command line */
	if (emit_prompt) {
	    printf("%s", prompt);
	    fflush(stdout);
	}
	if ((fgets(cmdline, MAXLINE, stdin) == NULL) && ferror(stdin))
	    app_error("fgets error");
	if (feof(stdin)) { /* End of file (ctrl-d) */
	    fflush(stdout);
	    exit(0);
	}

	/* Evaluate the command line */
	eval(cmdline);
	fflush(stdout);
	fflush(stdout);
    } 

    exit(0); /* control never reaches here */
}
  





/* 
 * eval - Evaluate the command line that the user has just typed in
 * 
 * If the user has requested a built-in command (quit, jobs, bg or fg)
 * then execute it immediately. Otherwise, fork a child process and
 * run the job in the context of the child. If the job is running in
 * the foreground, wait for it to terminate and then return.  Note:
 * each child process must have a unique process group ID so that our
 * background children don't receive SIGINT (SIGTSTP) from the kernel
 * when we type ctrl-c (ctrl-z) at the keyboard.  
*/
void eval(char *cmdline) 
{
    char *argv[MAXARGS];   /* Argument list execve() */
    char buf[MAXLINE];     /* Holds modified command line */
    int bg;                 /* Should the job run in bg or fg? */
    pid_t pid;             /* Process id*/
    sigset_t mask;
    strcpy(buf,cmdline);   
    bg=parseline(cmdline,argv);
    //跳过空白指令
    if(argv[0]==NULL)
    {
        return;
    }

    //Prevented from the completing condition, add the thing



    if(!builtin_cmd(argv))
    {
        

        //Prevented from the completing condition, add the thing
        //必须放在builtin_cmd里面，因为如果放在外面，在执行内部命令的时候，就不会释放这些锁了
        Sigemptyset(&mask);
        Sigaddset(&mask,SIGCHLD);
        Sigprocmask(SIG_BLOCK,&mask,NULL);

        if((pid=Fork())==0)         //创建子进程,Running the child process.
        {
            Sigprocmask(SIG_UNBLOCK,&mask,NULL);  //UNBLOCK the order
            if(setpgid(0,0)<0)
            {
                unix_error("eval: setpgid failed.\n");
            }
            Execve(argv[0],argv,environ);    //孩子进程自己执行他的应用程序
        }


        //Parent process:add new jobs and printf message
        else
        {
            if(bg)
                addjob(jobs,pid,BG,cmdline);
            else
                addjob(jobs,pid,FG,cmdline);
            Sigprocmask(SIG_UNBLOCK,&mask,NULL);    //UNBLOCK the order

            //父进程等待前置进程结束
            if(!bg)
            {
                waitfg(pid);
            }
            else
            {
                printf("[%d] (%d) %s",pid2jid(pid),pid,cmdline);
            }
        }
    }
}



/* 
 * parseline - Parse the command line and build the argv array.
 * 
 * Characters enclosed in single quotes are treated as a single
 * argument.  Return true if the user has requested a BG job, false if
 * the user has requested a FG job.  
 */
int parseline(const char *cmdline, char **argv) 
{
    static char array[MAXLINE]; /* holds local copy of command line */
    char *buf = array;          /* ptr that traverses command line */
    char *delim;                /* points to first space delimiter */
    int argc;                   /* number of args */
    int bg;                     /* background job? */

    strcpy(buf, cmdline);
    buf[strlen(buf)-1] = ' ';  /* replace trailing '\n' with space */
    while (*buf && (*buf == ' ')) /* ignore leading spaces */
	buf++;

    /* Build the argv list */
    argc = 0;
    if (*buf == '\'') {
	buf++;
	delim = strchr(buf, '\'');
    }
    else {
	delim = strchr(buf, ' ');
    }

    while (delim) {
	argv[argc++] = buf;
	*delim = '\0';
	buf = delim + 1;
	while (*buf && (*buf == ' ')) /* ignore spaces */
	       buf++;

	if (*buf == '\'') {
	    buf++;
	    delim = strchr(buf, '\'');
	}
	else {
	    delim = strchr(buf, ' ');
	}
    }
    argv[argc] = NULL;
    
    if (argc == 0)  /* ignore blank line */
	return 1;

    /* should the job run in the background? */
    if ((bg = (*argv[argc-1] == '&')) != 0) {
	argv[--argc] = NULL;
    }
    return bg;
}

/* 
 * builtin_cmd - If the user has typed a built-in command then execute
 *    it immediately.  
 */
 //quit, jobs, bg or fg
 //如果输出为true，说明进入的参数并且为内置函数
 //如果输出为false，说明执行的命令为外置函数
int builtin_cmd(char **argv) 
{
    if(!strcmp(argv[0],"quit"))   //退出命令
    {
        exit(0);
    }
    else if(!strcmp(argv[0],"&"))     /* Ignore singleton & */
    {
        return 1;
    }
    else if(!strcmp(argv[0],"jobs"))    //job order
    {
        listjobs(jobs);
        return 1;
    }
    else if(!strcmp(argv[0],"bg") || !strcmp(argv[0],"fg"))
    {
        do_bgfg(argv);
        return 1;
    }
    return 0;
}


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


//用于获取pid,jid的数字
int trunce(char *src)
{
    
    int len=strlen(src);
    char * dst=malloc(sizeof(char)*len*2);
    if(src[0]=='%')
        right(dst,src,len-1);
    else
        memcpy(dst,src,len);
    int result=atoi(dst);
    free(dst);
    return result;
}

/* 
 * do_bgfg - Execute the builtin bg and fg commands
 */
void do_bgfg(char **argv) 
{
    struct job_t* StpJob;
    char* id=argv[1];
    int jid;
    pid_t pid;



    //如果命令不存在
    if(id==NULL)
    {
        printf("%s command requireds pid or %%jobid argument\n",argv[0]);
        return;
    }



    //如果命令表示的是job
    if(id[0]=='%')
    {
        jid=trunce(id);
        if(!(StpJob=getjobjid(jobs,jid)))
        {
            printf("%s:No such job\n",id);
            return;
        }
    }
    
    // For a PID
    else if(isdigit(id[0]))
    {
        pid=trunce(id);
        if(!(StpJob=getjobpid(jobs,pid)))
        {
            printf("(%d):No such process\n",pid);
            return;
        }
    }
    else
    {
        printf("%s: argument must be a PID or %%jobid\n",argv[0]);
        return;
    }

    //发送continue信息
    if(kill(-(StpJob->pid),SIGCONT)<0)
    {
        if(errno!=ESRCH)
        {
            unix_error("kill error");
        }
    }

    //FG和BG执行两种操作

    if(!strcmp(argv[0],"bg"))
    {
        StpJob->state=BG;
        printf("[%d] (%d) %s",StpJob->jid,StpJob->pid,StpJob->cmdline);
    }
    else if(!strcmp(argv[0],"fg"))
    {
        StpJob->state=FG;
        waitfg(StpJob->pid);
    }
    else
    {
        printf("bg/fg error:%s\n",argv[0]);
    }
    return;
}

/* 
 * waitfg - Block until process pid is no longer the foreground process
 */
void waitfg(pid_t pid)
{
    while(pid==fgpid(jobs))
    {
        sleep(0);
    }
}

/*****************
 * Signal handlers
 *****************/

/* 
 * sigchld_handler - The kernel sends a SIGCHLD to the shell whenever
 *     a child job terminates (becomes a zombie), or stops because it
 *     received a SIGSTOP or SIGTSTP signal. The handler reaps all
 *     available zombie children, but doesn't wait for any other
 *     currently running children to terminate.  
 */
void sigchld_handler(int sig) 
{
    int status;
    pid_t pid;
    //Waiting for/ handling all of the child processes according to their status
    while((pid=waitpid(-1,&status,WNOHANG|WUNTRACED))>0)   /* Reap a zombie child */
    {
        if(WIFSTOPPED(status))
        {
             sigtstp_handler(-pid);
        }
        //WIFSIGNALED表示因为未被捕获的信号而中断，适用于子进程自己给自己发送KILL而中断
        else if(WIFSIGNALED(status))
        {
            //表示进程自己给自己发信号而造成的程序中止
              sigint_handler(-pid);
        }
        else if(WIFEXITED(status))
        {
            deletejob(jobs,pid);      /* Delete the child from the job list */
        }
    }
    if(errno!=ECHILD)
      unix_error("waitpid error");
    return;
}

/* 
 * sigint_handler - The kernel sends a SIGINT to the shell whenver the
 *    user types ctrl-c at the keyboard.  Catch it and send it along
 *    to the foreground job.  
 */
void sigint_handler(int sig) 
{
    pid_t pid=fgpid(jobs);
    int jid=pid2jid(pid);

    //只处理前台进程
    if(pid!=0)
    {
        //说明是进程通过kill函数发送的信号, 通过sigchld_handler发的信号
        if(pid==-sig)
        {
            printf("Job [%d] (%d) terminated by signal %d\n",pid2jid(-sig),-sig,2);
            deletejob(jobs,-sig);
        }
        // when sig<0, send SIGINT singal to all foreground process
        else if(sig==SIGINT)
        {
            kill(-pid,SIGINT);
            printf("Job [%d] (%d) terminated by signal %d\n",jid,pid,sig);
            deletejob(jobs,pid);
        }
        //只触发一次sigint_handler,实现对进程的发送信息，打印结果，删除任务
 
    }

    return;
}

/*
 * sigtstp_handler - The kernel sends a SIGTSTP to the shell whenever
 *     the user types ctrl-z at the keyboard. Catch it and suspend the
 *     foreground job by sending it a SIGTSTP.  
 */
void sigtstp_handler(int sig) 
{
    pid_t pid=fgpid(jobs);
    int jid=pid2jid(pid);
    //send fg job/ related process group signal
    if(pid!=0)
    {
        //通过ctrl-z发送信号
        if(sig==20)
        {
            printf("Job [%d] (%d) Stopped by signal %d\n",jid,pid,sig);
            getjobpid(jobs,pid)->state=ST;
            kill(-pid,SIGTSTP);
        }
        //通过自己发送kill函数发送信号
        else if(pid==-sig)
        {
            printf("Job [%d] (%d) Stopped by signal %d\n",jid,pid,20);
            getjobpid(jobs,pid)->state=ST;
        }
        //其他情况说明是已经停止的进程通过sigchild发送信息
    }
    return;
}

/*********************
 * End signal handlers
 *********************/




/***********************
 * Other helper routines
 ***********************/

/*
 * usage - print a help message
 */
void usage(void) 
{
    printf("Usage: shell [-hvp]\n");
    printf("   -h   print this message\n");
    printf("   -v   print additional diagnostic information\n");
    printf("   -p   do not emit a command prompt\n");
    exit(1);
}



/*
 * Signal - wrapper for the sigaction function
 */
handler_t *Signal(int signum, handler_t *handler) 
{
    struct sigaction action, old_action;

    action.sa_handler = handler;  
    sigemptyset(&action.sa_mask); /* block sigs of type being handled */
    action.sa_flags = SA_RESTART; /* restart syscalls if possible */

    if (sigaction(signum, &action, &old_action) < 0)
	unix_error("Signal error");
    return (old_action.sa_handler);
}

/*
 * sigquit_handler - The driver program can gracefully terminate the
 *    child shell by sending it a SIGQUIT signal.
 */
void sigquit_handler(int sig) 
{
    printf("Terminating after receipt of SIGQUIT signal\n");
    exit(1);
}




