#include<stdio.h>
#include<unistd.h>
#include<stdlib.h>
#include<string.h>
#include<sys/types.h>
#include<wait.h>

int main(){
    int fildes[2];
    pipe(fildes);
    pid_t child1,child2;
    char* str1="Child process 1 is sending a message!\n";
    char* str2="Child process 2 is sending a message!\n\0";
    char read_str[1001];
    child1=fork();

    if (child1){
        wait(NULL);
        child2=fork();
        if(child2){
            wait(NULL);
            close(fildes[1]);
            read(fildes[0],read_str,1001);
            printf("%s",read_str);
            exit(0);
        }else{
            close(fildes[0]);
            write(fildes[1],str2,strlen(str2)+1);
            exit(0);
        }
    }else{
        close(fildes[0]);
        write(fildes[1],str1,strlen(str1));
        exit(0);
    }
    return 0;
    
}
