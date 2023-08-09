#include<stdio.h>
#include <sys/types.h>


int main()
{
    pid_t pid;
    int i;
    for(i = 0; i < 5; i++)
    {
        pid = fork();
        if(pid == 0){
            printf("child %d\n", getpid());
            break;
        }else if(pid == -1){
            perror("pid error!\n");
        }else{
            continue;
        }
    }
    if(i == 5)
    {
        sleep(1);
        while (1)
        {
            int res = wait();
            if(res == -1)
                break;
            printf("wait one\n");
        }
        
        printf("parent!!\n");
    }
    
    return 0;
}