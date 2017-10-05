//处理字符串的程序

#define MAXNAME 100*sizeof(char)


/* 程序的结构体类型 */
struct hostmessage
{
    char* cwd;       //路径名
    char* hostname;    //主机名，系统名
    char* usrname;    //用户名
    int isroot;      //判断是否root,1为非root,0表示root
};


/* 进行字符串截取的程序 */
char * right(char *dst,char *src, int n);


/* 获取主机内容 */
struct hostmessage  *getmessage(void);

/* 打印对应的命令行符号 */
void printname(void);

/* 释放malloc hostmessage的内存 */
int freemessage(struct hostmessage* oldmessage);


