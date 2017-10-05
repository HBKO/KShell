//封装警告，大概率偏转的函数
#include <signal.h>


/* Sleep for a random period between [0,MAX_SLEEP] us */
#define MAX_SLEEP 100000

/* Macro that maps val into the range [0,RAND_MAX] */
#define CONVERT(val) (((double)val)/(double)RAND_MAX)


/* 针对于多进程的程序，这个fork可以进行父子进程50%的优先运行，可以更快的竞争问题暴露出来 */
pid_t Fork(void);

/* 对于sigemptyset进行警告封装 */
void Sigemptyset(sigset_t *mask);


/* 针对sigaddset()进行警告封装 */
void Sigaddset(sigset_t *mask,int signum);

/* 针对sigprocmask()进行警告封装 */
void Sigprocmask(int how, const sigset_t * set, sigset_t * oldset);


/* execve with error */
void Execve(char *string,char **argv,char ** environ);

/*
 * unix_error - unix-style error routine
 */
void unix_error(char *msg);

/*
 * app_error - application-style error routine
 */
void app_error(char *msg);